#include "GetBybitInstrumentsInfoHandler.hpp"

namespace Core::Handlers
{

void GetBybitInstrumentsInfoHandler::processRequestTradePairs(Instruments& instruments, const QJsonObject &result)
{
    QString category = result["category"].toString();
    QJsonArray list = result["list"].toArray();

    EnumHelpers::MarketType marketType = EnumHelpers::apiStringToMarketType(category);

    for (const auto& obj : std::as_const(list))
    {
        QJsonObject item = obj.toObject();
        InstrumentInfo info;

        info.m_category = marketType;
        info.m_symbol = item["symbol"].toString();
        info.m_baseCoin = item["baseCoin"].toString();
        info.m_quoteCoin = item["quoteCoin"].toString();
        info.m_status = item["status"].toString();

        QJsonObject priceFilter = item["priceFilter"].toObject();
        info.m_tickSize = priceFilter["tickSize"].toString();

        QJsonObject lotSizeFilter = item["lotSizeFilter"].toObject();

        if (category == "spot")
        {
            info.m_qtyStep = lotSizeFilter["basePrecision"].toString();

            info.m_minNotional = lotSizeFilter["minOrderAmt"].toString();
            info.m_minOrderQty = "";

            info.m_maxOrderQty = lotSizeFilter["maxLimitOrderQty"].toString();
            info.m_maxMktQty = lotSizeFilter["maxMarketOrderQty"].toString();

            info.m_maxLeverage = "1.0";
        }
        else if (category == "linear")
        {
            info.m_qtyStep = lotSizeFilter["qtyStep"].toString();
            info.m_minOrderQty = lotSizeFilter["minOrderQty"].toString();
            info.m_maxOrderQty = lotSizeFilter["maxOrderQty"].toString();
            info.m_maxMktQty = lotSizeFilter["maxMktOrderQty"].toString();
            info.m_minNotional = lotSizeFilter["minNotionalValue"].toString();

            QJsonObject leverageFilter = item["leverageFilter"].toObject();
            info.m_maxLeverage = leverageFilter["maxLeverage"].toString();
        }

        instruments.append(info);
    }
}

QUrlQuery GetBybitInstrumentsInfoHandler::buildRequest(const Tools::InstrumentsInfoRequest &req)
{
    QUrlQuery params;

    params.addQueryItem("category", EnumHelpers::marketTypeToApiString(req.m_category));

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

    Instruments instruments;
    processRequestTradePairs(instruments, result);

    emit signaller->instrumentsInfoPageReceived(instruments, nextPageCursor, requestContext);
}

} // namespace Core::Handlers
