#include "MarketDataMediator.hpp"
#include "Markets/Bybit/BybitMarketDataService.hpp"
#include "Markets/Bybit/BybitMarketDataStreamer.hpp"

namespace Core {

MarketDataMediator::MarketDataMediator(QObject* parent) : BaseMediator(parent)
{
    m_publicService = std::make_unique<BybitMarketDataService>();
    m_publicStreamer = std::make_unique<BybitMarketDataStreamer>(Tools::EnumHelpers::MarketType::Spot);
    m_marketDataRep = std::make_unique<Repository::MarketDataRepository>();

    bindService(m_publicService->signaller());
    bindStreamer(m_publicStreamer->signaller());

    connect(m_marketDataRep.get(), &Repository::MarketDataRepository::errorOccurred,
            this, &MarketDataMediator::errorOccurred);

    // Связываем Сеть (REST)
    connect(m_publicService->signaller(), &Service::MarketDataServiceSignals::allInstrumentsInfoReceived,
            this, &MarketDataMediator::onInstrumentsReady);

    connect(m_publicService->signaller(), &Service::MarketDataServiceSignals::klinesReceived,
            this, &MarketDataMediator::onKlinesReady);

    // Связываем публичный стрим
    connect(m_publicStreamer->signaller(), &Streamer::MarketDataStreamerSignals::klineUpdated,
            this, &MarketDataMediator::onKlineReceived);

    connect(m_publicStreamer->signaller(), &Streamer::MarketDataStreamerSignals::tickerUpdated,
            this, &MarketDataMediator::tickerReady);

    connect(m_publicStreamer->signaller(), &Streamer::MarketDataStreamerSignals::orderbookUpdated,
            this, &MarketDataMediator::orderbookReady);

    connect(m_publicStreamer->signaller(), &Streamer::MarketDataStreamerSignals::publicTradeUpdated,
            this, &MarketDataMediator::onPublicTradesReceived);

}

void MarketDataMediator::subscribe(const QString &symbol)
{
    m_publicStreamer->subscribeSymbol(symbol, {
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
    });
}

Tools::Instruments MarketDataMediator::readInstrumentsFromRepository(Tools::EnumHelpers::MarketType type)
{
    return m_marketDataRep->loadInstruments(type);
}

QList<Tools::Kline> MarketDataMediator::readKlinesFromRepository(const Tools::StdExport &std, const Tools::SpecialExportKlines &special)
{
    return m_marketDataRep->loadKlines(std.m_category,
                                       std.m_symbol,
                                       special.m_interval,
                                       special.m_start,
                                       special.m_end);
}

Tools::PublicTrades MarketDataMediator::readPublicTradesFromRepository(const Tools::StdExport& std)
{
    return m_marketDataRep->loadPublicTrades(std.m_category, std.m_symbol);
}

void MarketDataMediator::loadInstrumentsFromNetwork(Tools::EnumHelpers::MarketType type)
{
    emit messageSent("Loading instruments from network...");
    Tools::InstrumentsInfoRequest req;
    req.m_category = type;
    m_publicService->requestInstruments(req);
}

void MarketDataMediator::loadKlinesFromNetwork(const Tools::KlinesRequest& req)
{
    emit messageSent("Loading klines from network...");
    m_publicService->requestKlines(req);
}

void MarketDataMediator::init(bool isTestnet)
{
    m_publicService->init(isTestnet);
    m_publicStreamer->init(isTestnet);
    try {
        m_marketDataRep->init(isTestnet);
    } catch (const std::exception& e) {
        qCritical() << e.what();
    }
}

void MarketDataMediator::onInstrumentsReady(const Tools::Instruments &instruments)
{
    emit messageSent("Instruments ready: " + QString::number(instruments.size()));
    m_marketDataRep->saveInstruments(instruments);
    emit instrumentsReady(instruments);
}

void MarketDataMediator::onKlineReceived(const Tools::Kline& kline)
{
    if (kline.m_confirm)
    {
        m_marketDataRep->saveKlines(kline);
    }
    emit klineUpdated(kline);
}

void MarketDataMediator::onKlinesReady(const QList<Tools::Kline>& klines)
{
    emit messageSent("Klines ready: " + QString::number(klines.size()));
    m_marketDataRep->saveKlines(klines);
    emit historicalKlinesReady(klines);
}

void MarketDataMediator::onPublicTradesReceived(const Tools::PublicTrades& trades)
{
    m_marketDataRep->savePublicTrades(trades);
    emit tradesReady(trades);
}

}