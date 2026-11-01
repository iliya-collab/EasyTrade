#include "MarketDataService.hpp"
#include "Tools/RequestBuilders.hpp"
#include "Tools/ExportBuilders.hpp"
#include "Tools/Logging.hpp"

namespace Core
{

MarketDataService::MarketDataService(std::shared_ptr<MarketDataState> state,
                                 std::shared_ptr<MarketDataMediator> mediator,
                                 QObject *parent) :
    m_state(state), m_mediator(mediator), QObject(parent)
{
    connect(mediator.get(), &MarketDataMediator::tickerReady,
            state.get(), &MarketDataState::updateTicker, Qt::UniqueConnection);

    connect(mediator.get(), &MarketDataMediator::orderbookReady,
            state.get(), &MarketDataState::updateOrderbook, Qt::UniqueConnection);

    connect(mediator.get(), &MarketDataMediator::klineUpdated,
            state.get(), &MarketDataState::updateKline, Qt::UniqueConnection);

    connect(mediator.get(), &MarketDataMediator::historicalKlinesReady,
            state.get(), &MarketDataState::addHistoricalKlines, Qt::UniqueConnection);

    connect(mediator.get(), &MarketDataMediator::instrumentsReady,
            state.get(), &MarketDataState::updateInstruments, Qt::UniqueConnection);

    connect(mediator.get(), &MarketDataMediator::tradesReady,
            state.get(), &MarketDataState::updateTrades, Qt::UniqueConnection);

    connect(mediator.get(), &MarketDataMediator::pingMeasured,
            state.get(), &MarketDataState::updatePingMs, Qt::UniqueConnection);

    // --------------------------------------------------------------------------------------

    connect(mediator.get(), &MarketDataMediator::messageSent,
            this, &MarketDataService::messageReceived, Qt::UniqueConnection);

    connect(mediator.get(), &MarketDataMediator::streamerStarted,
            this, &MarketDataService::onStreamerStarted, Qt::UniqueConnection);

    connect(mediator.get(), &MarketDataMediator::streamerStopped,
            this, &MarketDataService::onStreamerStopped, Qt::UniqueConnection);

    connect(mediator.get(), &MarketDataMediator::downloadProgress,
            this, &MarketDataService::downloadProgress, Qt::UniqueConnection);

    connect(mediator.get(), &MarketDataMediator::errorOccurredWithId,
            this, &MarketDataService::onErrorOccurredWithId, Qt::UniqueConnection);

    connect(mediator.get(), &MarketDataMediator::errorOccurred,
            this, &MarketDataService::onErrorOccurred, Qt::UniqueConnection);
}

void MarketDataService::run()
{
    m_mediator->runStreamer();
}

void MarketDataService::restart()
{
    m_mediator->restartStreamer();
}

void MarketDataService::shutdown()
{
    m_mediator->stopStreamer();

    QEventLoop loop;
    QTimer::singleShot(5000, &loop, &QEventLoop::quit);

    connect(m_mediator.get(), &Core::MarketDataMediator::streamerStopped, &loop, &QEventLoop::quit, Qt::SingleShotConnection);

    loop.exec();

}

void MarketDataService::init(bool isTestnet)
{
    m_mediator->init(isTestnet);
}

void MarketDataService::selectCurrentInstrument(const Tools::InstrumentInfo &instrument)
{
    m_state->selectCurrentInstrument(instrument);
}

void MarketDataService::subscribeInstrument()
{
    const auto& instrument = m_state->getCurrentInstrument();
    m_mediator->subscribe(instrument.m_symbol);
}

void MarketDataService::loadInstruments(Core::Tools::EnumHelpers::MarketType type)
{
    auto res = m_mediator->readInstrumentsFromRepository(type);

    if (!res.isEmpty())
    {
        m_state->updateInstruments(res);
        return;
    }

    m_mediator->loadInstrumentsFromNetwork(type);
}

void MarketDataService::loadKlines(const QVariantMap& params)
{
    validateAndSend(params, Tools::buildKlinesRequest, [this](const Tools::KlinesRequest& req)
    {
        m_mediator->loadKlinesFromNetwork(req);
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
        emit errorOccurred("export: no klines in repository for the requested range");
        return;
    }

    const QUrl url(path);
    const QString localPath = url.isLocalFile() ? url.toLocalFile() : path;

    doExport(klines, format, localPath, [](const Tools::Kline& k) { return Tools::Exporter::toField(k); }, opts);
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
        emit errorOccurred("export: no trades in repository");
        return;
    }

    const QUrl url(path);
    const QString localPath = url.isLocalFile() ? url.toLocalFile() : path;

    doExport(trades, format, localPath, [](const Tools::PublicTradeItem& t) { return Tools::Exporter::toField(t); }, opts);
}

void MarketDataService::onErrorOccurredWithId(const QString& id, const QString &error)
{
    QString errMsg = id + " : " + error;
    emit errorOccurred(errMsg);
}

void MarketDataService::onStreamerStarted(const QString& id)
{
    emit streamerStarted(id);
}

void MarketDataService::onStreamerStopped(const QString& id)
{
    emit streamerStopped(id);
}

void MarketDataService::onErrorOccurred(const QString &error)
{
    emit errorOccurred(error);
}

}
