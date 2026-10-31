#include "BybitPublicTradeStreamHandler.hpp"

void Core::Handlers::BybitPublicTradeStreamHandler::handle(const QString& id, const QJsonObject &obj, Streamer::MarketDataStreamerSignals *signaller)
{
    if (!obj.contains("data") || !obj["data"].isArray())
    {
        emit signaller->errorOccurred(id, "[" + topic() + "] Invalid response structure!");
        return;
    }

    QJsonArray arrData = obj["data"].toArray();
    QString symbol = obj["topic"].toString().section('.', -1);
    Tools::EnumHelpers::MarketType category = Tools::EnumHelpers::apiStringToMarketType(id);

    Tools::PublicTrades publicTrades{};

    for (const auto& val : std::as_const(arrData))
    {
        QJsonObject itemData = val.toObject();

        Tools::PublicTradeItem publicTradeItem{};
        publicTradeItem.m_category = category;
        publicTradeItem.m_symbol = symbol;
        publicTradeItem.m_side = Tools::EnumHelpers::stringToOrderSide(itemData["S"].toString());
        publicTradeItem.m_price = itemData["p"].toString();
        publicTradeItem.m_volume = itemData["v"].toString();
        publicTradeItem.m_turnover = QString::number(publicTradeItem.m_price.toDouble() * publicTradeItem.m_volume.toDouble());
        publicTradeItem.m_tradeTime = itemData["T"].toVariant().toLongLong();

        publicTrades.append(publicTradeItem);
    }

    emit signaller->publicTradeUpdated(publicTrades);
}
