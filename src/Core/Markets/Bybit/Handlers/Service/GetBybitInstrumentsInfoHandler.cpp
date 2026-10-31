#include "GetBybitInstrumentsInfoHandler.hpp"

namespace Core::Handlers
{

void GetBybitInstrumentsInfoHandler::processRequestTradePairs(Tools::Instruments& instruments, const QJsonObject &result)
{
    QString category = result["category"].toString();
    QJsonArray list = result["list"].toArray();

    for (const auto& obj : std::as_const(list))
    {
        QJsonObject item = obj.toObject();
        Tools::InstrumentInfo info;
        info.m_category = Tools::EnumHelpers::apiStringToMarketType(category);
        info.m_symbol = item["symbol"].toString();
        info.m_baseCoin = item["baseCoin"].toString();
        info.m_quoteCoin = item["quoteCoin"].toString();
        instruments.append(info);
    }
}

QUrlQuery GetBybitInstrumentsInfoHandler::buildRequest(const Tools::InstrumentsInfoRequest &req)
{
    QUrlQuery params;

    params.addQueryItem("category", Tools::EnumHelpers::marketTypeToApiString(req.m_category));

    if (req.m_limit.has_value())
        params.addQueryItem("limit", QString::number(req.m_limit.value()));

    if (req.m_cursor.has_value() && !req.m_cursor->isEmpty())
        params.addQueryItem("cursor", req.m_cursor.value());

    return params;
}

void GetBybitInstrumentsInfoHandler::handle(const QJsonObject &data, const QVariant& requestContext, Service::MarketDataServiceSignals *signaller)
{
    if (!data.contains("retCode") || data["retCode"].toInt() != 0)
    {
        emit signaller->errorOccurred(QString("Error processing request [endpoint = %1]: " + data["retMsg"].toString()).arg(endpoint()));
        return;
    }

    QJsonObject result = data["result"].toObject();
    QString nextPageCursor = result["nextPageCursor"].toString();

    Tools::Instruments instruments;
    processRequestTradePairs(instruments, result);

    emit signaller->instrumentsInfoPageReceived(instruments, nextPageCursor, requestContext);
}

}
