#include "BybitOrderbookStreamHandler.hpp"

void Core::Handlers::BybitOrderbookStreamHandler::handle(const QString& id, const QJsonObject &obj, Streamer::MarketDataStreamerSignals *signaller)
{
    if (!obj.contains("data") || !obj["data"].isObject())
    {
        emit signaller->errorOccurred(id, "[" + topic() + "] Invalid response structure!");
        return;
    }

    QJsonObject data = obj["data"].toObject();
    QString symbol = data["s"].toString();
    QString type = obj["type"].toString();

    OrderbookInfo orderbook{};

    orderbook.m_category = EnumHelpers::apiStringToMarketType(id);
    orderbook.m_symbol = symbol;

    QJsonArray bidsArray = data.value("b").toArray();
    for (const auto& bidVal : std::as_const(bidsArray))
    {
        QJsonArray bid = bidVal.toArray();
        QString price = bid[0].toString();
        QString size = bid[1].toString();
        orderbook.m_bids.insert(price, size);
    }

    QJsonArray asksArray = data.value("a").toArray();
    for (const auto& askVal : std::as_const(asksArray))
    {
        QJsonArray ask = askVal.toArray();
        QString price = ask[0].toString();
        QString size = ask[1].toString();
        orderbook.m_asks.insert(price, size);
    }

    if (type == "snapshot")
        emit signaller->orderbookSnapshot(orderbook);
    else
        emit signaller->orderbookDeltaUpdated(orderbook);

    /*m_savedOrderbooks[symbol].m_category = EnumHelpers::apiStringToMarketType(id);
    m_savedOrderbooks[symbol].m_symbol = symbol;

    if (type == "snapshot")
        snapshotOrderbook(m_savedOrderbooks[symbol], orderbook);
    else
        deltaUpdateOrderbook(m_savedOrderbooks[symbol], orderbook);

    emit signaller->orderbookUpdated(m_savedOrderbooks[symbol]);*/
}


/*void Core::Handlers::BybitOrderbookStreamHandler::snapshotOrderbook(OrderbookInfo& oldOrderbook, const OrderbookInfo& newOrderbook)
{
    oldOrderbook.m_bids = newOrderbook.m_bids;
    oldOrderbook.m_asks = newOrderbook.m_asks;
}

void Core::Handlers::BybitOrderbookStreamHandler::deltaUpdateOrderbook(OrderbookInfo& oldOrderbook, const OrderbookInfo& newOrderbook)
{
    for (const auto& bidVal : newOrderbook.m_bids.asKeyValueRange())
    {
        QString price = bidVal.first;
        QString size = bidVal.second;

        if (qFuzzyIsNull(size.toDouble()) || size.toDouble() <= 0)
            oldOrderbook.m_bids.remove(price);
        else
            oldOrderbook.m_bids.insert(price, size);
    }

    for (const auto& askVal : newOrderbook.m_asks.asKeyValueRange())
    {
        QString price = askVal.first;
        QString size = askVal.second;

        if (qFuzzyIsNull(size.toDouble()) || size.toDouble() <= 0)
            oldOrderbook.m_asks.remove(price);
        else
            oldOrderbook.m_asks.insert(price, size);
    }
}*/