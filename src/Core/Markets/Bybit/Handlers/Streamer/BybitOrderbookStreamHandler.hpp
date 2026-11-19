#pragma once
#include "Streamer/IStreamHandler.hpp"

namespace Core::Handlers
{

    class BybitOrderbookStreamHandler : public Streamer::IMarketDataStreamHandler
    {
    public:

        static QString topic() { return "orderbook"; }

        void handle(const QString& id, const QJsonObject& obj, Streamer::MarketDataStreamerSignals* signaller) override;

    };

}

