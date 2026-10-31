#pragma once
#include "Streamer/IStreamHandler.hpp"

namespace Core::Handlers
{

class BybitKlineStreamHandler : public Streamer::IMarketDataStreamHandler
{
public:

    static QString topic() { return "kline"; }

    void handle(const QString& id, const QJsonObject& obj, Streamer::MarketDataStreamerSignals* signaller) override;
};

}


