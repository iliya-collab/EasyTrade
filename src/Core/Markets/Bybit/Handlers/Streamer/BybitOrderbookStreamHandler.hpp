#pragma once
#include "Streamer/IStreamHandler.hpp"

namespace Core::Handlers
{

    class BybitOrderbookStreamHandler : public Streamer::IMarketDataStreamHandler
    {
    private:

        Tools::Orderbook m_savedOrderbook{};

        void snapshotOrderbook(Tools::Orderbook& oldOrderbook, const Tools::Orderbook& newOrderbook);
        void deltaUpdateOrderbook(Tools::Orderbook& oldOrderbook, const Tools::Orderbook& newOrderbook);

    public:

        static QString topic() { return "orderbook"; }

        void handle(const QString& id, const QJsonObject& obj, Streamer::MarketDataStreamerSignals* signaller) override;

    };

}

