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

    void tickerUpdated(const TickerInfo& newTicker);
    void orderbookSnapshot(const OrderbookInfo& snapshot);
    void orderbookDeltaUpdated(const OrderbookInfo& deltaOrderbook);
    void klineUpdated(const KlineInfo& newKline);
    void publicTradeUpdated(const PublicTrades& newPublicTrades);

};

}

#endif // MARKETDATASTREAMERSIGNALS_HPP
