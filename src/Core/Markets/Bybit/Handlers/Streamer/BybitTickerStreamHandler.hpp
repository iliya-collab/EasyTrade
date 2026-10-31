#pragma once
#include "Streamer/IStreamHandler.hpp"

namespace Core::Handlers
{

class BybitTickerStreamHandler : public Streamer::IMarketDataStreamHandler
{
public:

    static QString topic() { return "tickers"; }

    void handle(const QString& id, const QJsonObject& obj, Streamer::MarketDataStreamerSignals* signaller) override;

};

}

