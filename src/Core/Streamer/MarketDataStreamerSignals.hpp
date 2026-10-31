#ifndef MARKETDATASTREAMERSIGNALS_HPP
#define MARKETDATASTREAMERSIGNALS_HPP
#include "StreamerSignals.hpp"
#include "Tools/StdTypes.hpp"

namespace Core::Streamer
{

class MarketDataStreamerSignals : public StreamerSignals
{
    Q_OBJECT

public:

    explicit MarketDataStreamerSignals(QObject* parent = nullptr) : StreamerSignals(parent) {}

signals:

    void tickerUpdated(const Core::Tools::Ticker& newTicker);
    void orderbookUpdated(const Core::Tools::Orderbook& newOrderBook);
    void klineUpdated(const Core::Tools::Kline& newKline);
    void publicTradeUpdated(const Core::Tools::PublicTrades& newPublicTrades);

};

}

#endif // MARKETDATASTREAMERSIGNALS_HPP
