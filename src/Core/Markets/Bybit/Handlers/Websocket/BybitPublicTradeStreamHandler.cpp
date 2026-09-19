#include "BybitPublicTradeStreamHandler.hpp"

void Core::Markets::BybitPublicTradeStreamHandler::handle(const QJsonObject &obj, IMarketDataStreamer *streamer)
{
    if (!obj.contains("data") || !obj["data"].isArray())
    {
        emit streamer->errorOccurred(streamer->id(), "[" + topic() + "] Invalid response structure!");
        return;
    }

    QJsonArray arrData = obj["data"].toArray();
    QString symbol = obj["topic"].toString().section('.', -1);
    Tools::MarketType category = Tools::stringToMarketType(streamer->id());

    Tools::PublicTrades publicTrades{};

    for (const auto& val : std::as_const(arrData))
    {
        QJsonObject itemData = val.toObject();

        Tools::PublicTradeItem publicTradeItem{};
        publicTradeItem.m_category = category;
        publicTradeItem.m_symbol = symbol;
        publicTradeItem.m_side = Tools::stringToOrderSide(itemData["S"].toString());
        publicTradeItem.m_price = itemData["p"].toString();
        publicTradeItem.m_volume = itemData["v"].toString();
        publicTradeItem.m_turnover = QString::number(publicTradeItem.m_price.toDouble() * publicTradeItem.m_volume.toDouble());
        publicTradeItem.m_tradeTime = itemData["T"].toVariant().toLongLong();

        publicTrades.append(publicTradeItem);
    }

    emit streamer->publicTradeUpdated(publicTrades);
}
