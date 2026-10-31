#include "BaseMarketDataStreamer.hpp"
#include <QObject>

namespace Core::Streamer
{

BaseMarketDataStreamer::BaseMarketDataStreamer(std::unique_ptr<Tools::BaseWebSocket> websocket, QObject* parent)
    : BaseStreamer(std::move(websocket), parent) {}

void BaseMarketDataStreamer::subscribeSymbol(const QString &symbol, QSet<PublicStreams> streams)
{
    QStringList names;
    for (auto s : streams)
        names << createStream(symbol, s);
    addStreams(names);
}

void BaseMarketDataStreamer::unsubscribeSymbol(const QString &symbol, QSet<PublicStreams> streams)
{
    QStringList names;
    for (auto s : streams)
        names << createStream(symbol, s);
    removeStreams(names);
}

QString BaseMarketDataStreamer::createStream(const QString& symbol, PublicStreams stream)
{
    if (isKlineStream(stream))
        return createKlineStream(symbol, klineInterval(stream));

    if (isOrderbookStream(stream))
        return createOrderbookStream(symbol, orderbookDepth(stream));

    switch (stream)
    {
    case PublicStreams::Ticker:
        return createTickerStream(symbol);
    case PublicStreams::PublicTrade:
        return createPublicTradeStream(symbol);
    default:
        return QString();
    }
}

}