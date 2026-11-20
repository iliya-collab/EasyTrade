#include "MarketDataService.hpp"
#include "Tools/RequestBuilders.hpp"
#include "Tools/ExportBuilders.hpp"
#include "Tools/Logging.hpp"

namespace Core
{

MarketDataService::MarketDataService(std::shared_ptr<MarketDataState> state,
                                 std::shared_ptr<MarketDataMediator> mediator,
                                 QObject *parent) :
    m_state(state), m_mediator(mediator), BaseService(parent)
{
    connect(mediator.get(), &MarketDataMediator::tickerReady,
            state.get(), &MarketDataState::updateTicker);

    connect(mediator.get(), &MarketDataMediator::snapshotOrderbookReady,
            state.get(), &MarketDataState::snapshotOrderbook);

    connect(mediator.get(), &MarketDataMediator::deltaUpdateOrderbookReady,
            state.get(), &MarketDataState::deltaUpdateOrderbook);

    connect(mediator.get(), &MarketDataMediator::klineUpdated,
            state.get(), &MarketDataState::updateKline);

    connect(mediator.get(), &MarketDataMediator::historicalKlinesReady,
            state.get(), &MarketDataState::addHistoricalKlines);

    connect(mediator.get(), &MarketDataMediator::tradesReady,
            state.get(), &MarketDataState::updateTrades);

    // --------------------------------------------------------------------------------------

    connect(mediator.get(), &MarketDataMediator::instrumentsReady,
            state.get(), &MarketDataState::uploadInstruments);

    connect(mediator.get(), &MarketDataMediator::pingMeasured,
            state.get(), &MarketDataState::updatePingMs);

    // --------------------------------------------------------------------------------------

    connect(mediator.get(), &MarketDataMediator::messageSent,
            this, &MarketDataService::messageReceived);

    connect(mediator.get(), &MarketDataMediator::streamerStarted,
            this, &MarketDataService::onStreamerStarted);

    connect(mediator.get(), &MarketDataMediator::streamerStopped,
            this, &MarketDataService::onStreamerStopped);

    connect(mediator.get(), &MarketDataMediator::downloadProgress,
            this, &MarketDataService::downloadProgress);

    connect(mediator.get(), &MarketDataMediator::errorOccurredWithId,
            this, &MarketDataService::onErrorOccurredWithId);

    connect(mediator.get(), &MarketDataMediator::errorOccurred,
            this, &MarketDataService::onErrorOccurred);
}

void MarketDataService::run()
{
    postTo(m_mediator.get(), [m = m_mediator.get()]() { m->runAllStreams(); });
}

void MarketDataService::restart()
{
    postTo(m_mediator.get(), [m = m_mediator.get()]() { m->restartAllStreams(); });
}

void MarketDataService::shutdown()
{
    QEventLoop loop;
    QTimer::singleShot(5000, &loop, &QEventLoop::quit);

    connect(m_mediator.get(), &Core::MarketDataMediator::streamerStopped,
            &loop, &QEventLoop::quit, Qt::SingleShotConnection);

    postTo(m_mediator.get(), [m = m_mediator.get()]() { m->stopAllStreams(); });

    loop.exec();

}

void MarketDataService::init(bool isTestnet)
{
    postTo(m_mediator.get(), [m = m_mediator.get(), isTestnet]() { m->init(isTestnet); });
}

void MarketDataService::openInstrument(const InstrumentInfo &inst)
{
    if (m_state->isOpened(inst))
    {
        emit messageReceived(QLatin1String("Instrument %1 was already opened").arg(inst.id()));
        return;
    }

    m_state->openInstrument(inst);
    postTo(m_mediator.get(), [m = m_mediator.get(), inst] {
        m->subscribe(inst.m_category, inst.m_symbol); });
    emit messageReceived(QLatin1String("Instrument %1 was opened").arg(inst.id()));
}

void MarketDataService::closeInstrument(const InstrumentInfo& inst)
{
    if (!m_state->isOpened(inst))
    {
        emit messageReceived(QLatin1String("Instrument %1 was already closed").arg(inst.id()));
        return;
    }

    m_state->closeInstrument(inst);
    postTo(m_mediator.get(), [m = m_mediator.get(), inst] {
        m->unsubscribe(inst.m_category, inst.m_symbol); });
    emit messageReceived(QLatin1String("Instrument %1 was closed").arg(inst.id()));
}

void MarketDataService::selectCurrentInstrument(const InstrumentInfo &inst)
{
    if (!m_state->instrument(inst))
        openInstrument(inst);

    m_state->setCurrent(inst);
}

void MarketDataService::loadInstruments(EnumHelpers::MarketType type)
{
    postTo(m_mediator.get(), [m = m_mediator.get(), type]() {
        m->loadInstruments(type); });
}

void MarketDataService::loadKlines(const QVariantMap& params)
{
    validateAndSend(params, Tools::buildKlinesRequest, [this](const Tools::KlinesRequest& req) {
        postTo(m_mediator.get(), [m = m_mediator.get(), req] { m->loadKlines(req); });
    });
}

void MarketDataService::exportKlines(const QString &path,
                                     Tools::Exporter::ExportFormat format,
                                     const QVariantMap &opts,
                                     const QVariantMap& params)
{
    auto reqStd = Tools::buildStdExport(params);
    if (!reqStd.has_value())
    {
        emit errorOccurred(reqStd.error());
        return;
    }

    auto reqSpecial = Tools::buildSpecialExportKlines(params);
    if (!reqSpecial.has_value())
    {
        emit errorOccurred(reqSpecial.error());
        return;
    }

    const QUrl url(path);
    const QString localPath = url.isLocalFile() ? url.toLocalFile() : path;

    postTo(m_mediator.get(), [=, this, self = QPointer(this), m = m_mediator.get()] {
        auto klines = m->readKlinesFromRepository(*reqStd, *reqSpecial);

        QMetaObject::invokeMethod(self, [=, this] {
            if (klines.isEmpty())
            {
                emit warningOccurred("[export] no klines");
                return;
            }
            doExport(klines, format, localPath,[](const KlineInfo& k) {
                return Tools::Exporter::toField(k); }, opts);
        });
    });
}

void MarketDataService::exportTrades(const QString &path, Tools::Exporter::ExportFormat format, const QVariantMap &opts, const QVariantMap &params)
{
    auto reqStd = Tools::buildStdExport(params);
    if (!reqStd.has_value())
    {
        emit errorOccurred(reqStd.error());
        return;
    }

    const QUrl url(path);
    const QString localPath = url.isLocalFile() ? url.toLocalFile() : path;

    postTo(m_mediator.get(), [=, self = QPointer(this), m = m_mediator.get()] {
        auto trades = m_mediator->readPublicTradesFromRepository(reqStd.value());

        QMetaObject::invokeMethod(self, [=] {
            if (trades.isEmpty())
            {
                emit warningOccurred("[export] no trades");
                return;
            }
            doExport(trades, format, localPath, [](const PublicTradeInfo& t) {
                return Tools::Exporter::toField(t); }, opts);
        });
    });
}

void MarketDataService::onErrorOccurredWithId(const QString& id, const QString &error)
{
    QString errMsg = id + " : " + error;
    emit errorOccurred(errMsg);
}

void MarketDataService::onStreamerStarted(const QString& id)
{
    emit messageReceived(QLatin1String("Streamer (id : %1) started").arg(id));
    emit streamerStarted(id);
}

void MarketDataService::onStreamerStopped(const QString& id)
{
    emit messageReceived(QLatin1String("Streamer (id : %1) stopped").arg(id));
    emit streamerStopped(id);
}

void MarketDataService::onErrorOccurred(const QString &error)
{
    emit errorOccurred(error);
}

}
