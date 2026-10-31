#pragma once
#include "IMarketDataStreamer.hpp"
#include "BaseStreamer.hpp"

namespace Core::Streamer
{

class BaseMarketDataStreamer : public BaseStreamer<MarketDataStreamerSignals, IMarketDataStreamer>
{
    Q_OBJECT
public:

    explicit BaseMarketDataStreamer(std::unique_ptr<Tools::BaseWebSocket> websocket, QObject* parent = nullptr);

    void subscribeSymbol(const QString& symbol, QSet<PublicStreams> streams) override;
    void unsubscribeSymbol(const QString& symbol, QSet<PublicStreams> streams) override;

protected:

    QString createStream(const QString& symbol, PublicStreams stream);

};

}