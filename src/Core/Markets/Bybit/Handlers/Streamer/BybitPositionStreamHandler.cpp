#include "BybitPositionStreamHandler.hpp"

void Core::Handlers::BybitPositionStreamHandler::handle(const QString& id, const QJsonObject &data, Streamer::PrivateStreamerSignals *signaller)
{
    if (!data.contains("data") || !data["data"].isArray())
    {
        emit signaller->errorOccurred(id, "[" + topic() + "] Invalid response structure!");
        return;
    }

    QJsonArray dataArray = data["data"].toArray();
    if (dataArray.isEmpty())
        return;

    for (const auto& iData : std::as_const(dataArray))
    {
        QJsonObject dataObj = iData.toObject();

        PositionInfo position{};

        position.m_category = EnumHelpers::apiStringToMarketType(dataObj["category"].toString());
        position.m_symbol = dataObj["symbol"].toString();
        QString strSide = dataObj["side"].toString();
        if (strSide == "Buy")
            position.m_side = EnumHelpers::OrderSide::Buy;
        else if (strSide == "Sell")
            position.m_side = EnumHelpers::OrderSide::Sell;
        else
        {
            if (position.m_positionIdx == 1)
                position.m_side = EnumHelpers::OrderSide::Buy;
            else if (position.m_positionIdx == 2)
                position.m_side = EnumHelpers::OrderSide::Sell;
            else
                position.m_side = EnumHelpers::OrderSide::Unknown;
        }
        position.m_size = dataObj["size"].toString();
        position.m_positionIdx = dataObj["positionIdx"].toInt();

        position.m_avgPrice          = dataObj["entryPrice"].toString();
        position.m_liqPrice          = dataObj["liqPrice"].toString();
        position.m_markPrice         = dataObj["markPrice"].toString();
        position.m_leverage          = dataObj["leverage"].toString();
        position.m_positionIM        = dataObj["positionIM"].toString();
        position.m_positionMM        = dataObj["positionMM"].toString();
        position.m_unrealisedPnl     = dataObj["unrealisedPnl"].toString();
        position.m_cumRealisedPnl    = dataObj["cumRealisedPnl"].toString();

        position.m_adlRankIndicator  = dataObj["adlRankIndicator"].toInt();

        emit signaller->positionUpdated(position);
    }
}
