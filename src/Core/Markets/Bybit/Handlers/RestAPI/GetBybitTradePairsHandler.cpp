#include "GetBybitTradePairsHandler.hpp"

namespace Core::Markets
{

    void GetBybitTradePairsHandler::processRequestTradePairs(QList<Tools::TradeInfo> &pairs, const QJsonObject &data)
    {
        QJsonObject result = data["result"].toObject();
        QString category = result["category"].toString();
        QJsonArray list = result["list"].toArray();

        for (const auto& obj : std::as_const(list))
        {
            QJsonObject item = obj.toObject();
            Tools::TradeInfo info;
            info.m_category = Tools::EnumHelpers::apiStringToMarketType(category);
            info.m_symbol = item["symbol"].toString();
            info.m_baseCoin = item["baseCoin"].toString();
            info.m_quoteCoin = item["quoteCoin"].toString();
            pairs.append(info);
        }
    }

    void GetBybitTradePairsHandler::handle(const QJsonObject &data, const QVariant& requestContext, MarketDataServiceSignals *signaller)
    {
        if (!data.contains("retCode") || data["retCode"].toInt() != 0)
        {
            emit signaller->errorOccurred(QString("Error processing request [endpoint = %1]: " + data["retMsg"].toString()).arg(endpoint()));
            return;
        }

        QList<Tools::TradeInfo> pairs;
        processRequestTradePairs(pairs, data);
        emit signaller->tradePairsReceived(pairs);
    }

}
