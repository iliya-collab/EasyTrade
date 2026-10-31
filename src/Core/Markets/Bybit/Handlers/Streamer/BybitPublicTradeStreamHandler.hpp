#pragma once
#include "Streamer/IStreamHandler.hpp"

namespace Core::Handlers
{

    class BybitPublicTradeStreamHandler : public Streamer::IMarketDataStreamHandler
    {
    public:

        static QString topic() { return "publicTrade"; }

        void handle(const QString& id, const QJsonObject& obj, Streamer::MarketDataStreamerSignals* signaller) override;
    };

}

