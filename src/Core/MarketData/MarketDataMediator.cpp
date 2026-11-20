#include "MarketDataMediator.hpp"
#include "Markets/Bybit/BybitMarketDataService.hpp"
#include "Markets/Bybit/BybitMarketDataStreamer.hpp"

namespace Core {

static QSet<Streamer::PublicStreams> defaultStreams()
{
    return {
        Streamer::PublicStreams::Ticker,
        Streamer::PublicStreams::Orderbook50,
        Streamer::PublicStreams::Kline1m,
        Streamer::PublicStreams::Kline3m,
        Streamer::PublicStreams::Kline5m,
        Streamer::PublicStreams::Kline15m,
        Streamer::PublicStreams::Kline30m,
        Streamer::PublicStreams::Kline1h,
        Streamer::PublicStreams::Kline2h,
        Streamer::PublicStreams::Kline4h,
        Streamer::PublicStreams::Kline6h,
        Streamer::PublicStreams::Kline12h,
        Streamer::PublicStreams::Kline1D,
        Streamer::PublicStreams::Kline1W,
        Streamer::PublicStreams::Kline1M,
        Streamer::PublicStreams::PublicTrade
    };
}

MarketDataMediator::MarketDataMediator(QObject* parent) : MultiMediator(parent)
{
    m_publicService = std::make_unique<BybitMarketDataService>(this);
    m_marketDataRep = std::make_unique<Repository::MarketDataRepository>(this);

    m_pool.addStream(std::make_unique<BybitMarketDataStreamer>(Eh::MarketType::Spot, this));
    m_pool.addStream(std::make_unique<BybitMarketDataStreamer>(Eh::MarketType::Linear, this));

    connect(m_marketDataRep.get(), &Repository::MarketDataRepository::errorOccurred,
            this, &MarketDataMediator::errorOccurred);

    // Связываем Сеть (REST)
    bindService(m_publicService->signaller());

    connect(m_publicService->signaller(), &Service::MarketDataServiceSignals::allInstrumentsInfoReceived,
            this, &MarketDataMediator::onInstrumentsReady);

    connect(m_publicService->signaller(), &Service::MarketDataServiceSignals::klinesReceived,
            this, &MarketDataMediator::onKlinesReady);

    // Связываем публичные стримы в пуле соединений
    for (auto* s : m_pool.streams())
    {
        auto* mds = dynamic_cast<Streamer::IMarketDataStreamer*>(s);
        if (!mds)
            continue;

        bindStreamer(mds->signaller());

        connect(mds->signaller(), &Streamer::MarketDataStreamerSignals::klineUpdated,
                this, &MarketDataMediator::onKlineReceived);

        connect(mds->signaller(), &Streamer::MarketDataStreamerSignals::tickerUpdated,
                this, &MarketDataMediator::tickerReady);

        connect(mds->signaller(), &Streamer::MarketDataStreamerSignals::orderbookSnapshot,
                this, &MarketDataMediator::snapshotOrderbookReady);

        connect(mds->signaller(), &Streamer::MarketDataStreamerSignals::orderbookDeltaUpdated,
                this, &MarketDataMediator::deltaUpdateOrderbookReady);

        connect(mds->signaller(), &Streamer::MarketDataStreamerSignals::publicTradeUpdated,
                this, &MarketDataMediator::onPublicTradesReceived);
    }


}

void MarketDataMediator::unsubscribe(EnumHelpers::MarketType market, const QString &symbol)
{
    auto* mds = dynamic_cast<Streamer::IMarketDataStreamer*>(streamerFor(market));
    if (!mds)
    {
        emit errorOccurred("[unsubscribe] No streamer for market: " + Eh::marketTypeToApiString(market));
        return;
    }

    mds->unsubscribeSymbol(symbol, defaultStreams());
}

void MarketDataMediator::subscribe(EnumHelpers::MarketType market, const QString &symbol)
{
    auto* mds = dynamic_cast<Streamer::IMarketDataStreamer*>(streamerFor(market));
    if (!mds)
    {
        emit errorOccurred("[subscribe] No streamer for market: " + Eh::marketTypeToApiString(market));
        return;
    }

    mds->subscribeSymbol(symbol, defaultStreams());
}

Klines MarketDataMediator::readKlinesFromRepository(const Tools::StdExport &std, const Tools::SpecialExportKlines &special)
{
    return m_marketDataRep->loadKlines(std.m_category,
                                       std.m_symbol,
                                       special.m_interval,
                                       special.m_start,
                                       special.m_end);
}

PublicTrades MarketDataMediator::readPublicTradesFromRepository(const Tools::StdExport& std)
{
    return m_marketDataRep->loadPublicTrades(std.m_category, std.m_symbol);
}

void MarketDataMediator::loadInstruments(EnumHelpers::MarketType type)
{
    emit messageSent("Loading instruments...");
    Tools::InstrumentsInfoRequest req;
    req.m_category = type;
    m_publicService->requestInstruments(req);
}

void MarketDataMediator::loadKlines(const Tools::KlinesRequest& req)
{
    emit messageSent("Loading klines...");
    m_publicService->requestKlines(req);
}

void MarketDataMediator::init(bool isTestnet)
{
    m_publicService->init(isTestnet);
    for (auto* s : m_pool.streams())
    {
        auto* mds = dynamic_cast<Streamer::IMarketDataStreamer*>(s);
        if (!mds)
            continue;
        mds->init(isTestnet);
    }
    try {
        m_marketDataRep->init(isTestnet);
    } catch (const std::exception& e) {
        qCritical() << e.what();
    }
}

ConnectionsPool &MarketDataMediator::pool() { return m_pool; }

const ConnectionsPool &MarketDataMediator::cpool() const { return m_pool; }

void MarketDataMediator::onInstrumentsReady(const Instruments &instruments)
{
    emit messageSent("Instruments ready: " + QString::number(instruments.size()));
    emit instrumentsReady(instruments);
}

void MarketDataMediator::onKlineReceived(const KlineInfo& kline)
{
    if (kline.m_confirm)
    {
        m_marketDataRep->saveKlines(kline);
    }
    emit klineUpdated(kline);
}

void MarketDataMediator::onKlinesReady(const Klines& klines)
{
    emit messageSent("Klines ready: " + QString::number(klines.size()));
    m_marketDataRep->saveKlines(klines);
    emit historicalKlinesReady(klines);
}

void MarketDataMediator::onPublicTradesReceived(const PublicTrades& trades)
{
    m_marketDataRep->savePublicTrades(trades);
    emit tradesReady(trades);
}

}