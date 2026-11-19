#include "GetBybitFeeRateHandler.hpp"

QUrlQuery Core::Handlers::GetBybitFeeRateHandler::buildRequest(const Tools::FeeRateRequest &req)
{
    QUrlQuery params;

    params.addQueryItem("category", EnumHelpers::marketTypeToApiString(req.m_category));
    params.addQueryItem("symbol", req.m_symbol);

    return params;
}

void Core::Handlers::GetBybitFeeRateHandler::handle(const QJsonObject &data, const QVariant &requestContext, Service::PrivateServiceSignals *signaller)
{
    if (!data.contains("retCode") || data["retCode"].toInt() != 0)
    {
        emit signaller->errorOccurred(QString("Error processing request [endpoint = %1]: " + data["retMsg"].toString()).arg(endpoint()));
        return;
    }

    QJsonObject result = data["result"].toObject();

    QString category = result["category"].toString();
    QJsonArray list = result["list"].toArray();

    EnumHelpers::MarketType marketType = EnumHelpers::apiStringToMarketType(category);

    FeeRatesInfo rates;

    for (const auto& obj : std::as_const(list))
    {
        QJsonObject item = obj.toObject();

        rates.m_category = marketType;
        rates.m_symbol = item["symbol"].toString();
        rates.m_makerFeeRate = item["makerFeeRate"].toString();
        rates.m_takerFeeRate = item["takerFeeRate"].toString();
    }

    emit signaller->feeRatesReceived(rates);
}
