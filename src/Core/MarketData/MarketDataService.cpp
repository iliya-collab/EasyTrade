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
    m_mediator->runAllStreams();
}

void MarketDataService::restart()
{
    m_mediator->restartAllStreams();
}

void MarketDataService::shutdown()
{
    m_mediator->stopAllStreams();

    QEventLoop loop;
    QTimer::singleShot(5000, &loop, &QEventLoop::quit);

    connect(m_mediator.get(), &Core::MarketDataMediator::streamerStopped, &loop, &QEventLoop::quit, Qt::SingleShotConnection);

    loop.exec();

}

void MarketDataService::init(bool isTestnet)
{
    m_mediator->init(isTestnet);
}

void MarketDataService::openInstrument(const InstrumentInfo &inst)
{
    if (m_state->isOpened(inst))
    {
        emit messageReceived(QLatin1String("Instrument %1 was already opened").arg(inst.id()));
        return;
    }

    m_state->openInstrument(inst);
    m_mediator->subscribe(inst.m_category, inst.m_symbol);
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
    m_mediator->unsubscribe(inst.m_category, inst.m_symbol);
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
    m_mediator->loadInstruments(type);
}

void MarketDataService::loadKlines(const QVariantMap& params)
{
    validateAndSend(params, Tools::buildKlinesRequest, [this](const Tools::KlinesRequest& req)
    {
        m_mediator->loadKlines(req);
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

    /*coreWarn() << "From" << QDateTime::fromMSecsSinceEpoch(reqSpecial.value().m_start).toString("yyyy-MM-dd hh:mm")
               << "To" << QDateTime::fromMSecsSinceEpoch(reqSpecial.value().m_end).toString("yyyy-MM-dd hh:mm");*/
    auto klines = m_mediator->readKlinesFromRepository(reqStd.value(), reqSpecial.value());
    if (klines.isEmpty())
    {
        emit warningOccurred("[export] no klines in repository for the requested range");
        return;
    }

    const QUrl url(path);
    const QString localPath = url.isLocalFile() ? url.toLocalFile() : path;

    doExport(klines, format, localPath, [](const KlineInfo& k) { return Tools::Exporter::toField(k); }, opts);
}

void MarketDataService::exportTrades(const QString &path, Tools::Exporter::ExportFormat format, const QVariantMap &opts, const QVariantMap &params)
{
    auto reqStd = Tools::buildStdExport(params);
    if (!reqStd.has_value())
    {
        emit errorOccurred(reqStd.error());
        return;
    }

    auto trades = m_mediator->readPublicTradesFromRepository(reqStd.value());
    if (trades.isEmpty())
    {
        emit warningOccurred("[export] no trades in repository");
        return;
    }

    const QUrl url(path);
    const QString localPath = url.isLocalFile() ? url.toLocalFile() : path;

    doExport(trades, format, localPath, [](const PublicTradeInfo& t) { return Tools::Exporter::toField(t); }, opts);
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
