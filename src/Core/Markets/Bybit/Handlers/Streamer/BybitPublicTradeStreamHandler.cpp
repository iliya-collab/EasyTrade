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
    EnumHelpers::MarketType category = EnumHelpers::apiStringToMarketType(id);

    PublicTrades publicTrades{};

    for (const auto& val : std::as_const(arrData))
    {
        QJsonObject itemData = val.toObject();

        PublicTradeInfo publicTrade{};
        publicTrade.m_category = category;
        publicTrade.m_symbol = symbol;
        publicTrade.m_side = EnumHelpers::stringToOrderSide(itemData["S"].toString());
        publicTrade.m_price = itemData["p"].toString();
        publicTrade.m_volume = itemData["v"].toString();
        publicTrade.m_turnover = QString::number(publicTrade.m_price.toDouble() * publicTrade.m_volume.toDouble());
        publicTrade.m_tradeTime = itemData["T"].toVariant().toLongLong();

        publicTrades.append(publicTrade);
    }

    emit signaller->publicTradeUpdated(publicTrades);
}
