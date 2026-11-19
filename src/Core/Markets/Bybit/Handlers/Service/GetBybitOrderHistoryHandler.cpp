#include "GetBybitOrderHistoryHandler.hpp"

QUrlQuery Core::Handlers::GetBybitOrderHistoryHandler::buildRequest(const Tools::OrderHistoryRequest &req)
{
    QUrlQuery params;

    // Обязательный параметр
    params.addQueryItem("category", EnumHelpers::marketTypeToApiString(req.m_category));

    // Фильтры по инструменту
    if (req.m_symbol.has_value() && !req.m_symbol->isEmpty())
        params.addQueryItem("symbol", req.m_symbol.value());

    if (req.m_baseCoin.has_value() && !req.m_baseCoin->isEmpty())
        params.addQueryItem("baseCoin", req.m_baseCoin.value());

    if (req.m_settleCoin.has_value() && !req.m_settleCoin->isEmpty())
        params.addQueryItem("settleCoin", req.m_settleCoin.value());

    // Идентификаторы ордера
    if (req.m_orderId.has_value() && !req.m_orderId->isEmpty())
        params.addQueryItem("orderId", req.m_orderId.value());

    if (req.m_orderLinkId.has_value() && !req.m_orderLinkId->isEmpty())
        params.addQueryItem("orderLinkId", req.m_orderLinkId.value());

    // Тип и статус ордера
    if (req.m_orderFilter.has_value() && !req.m_orderFilter->isEmpty())
        params.addQueryItem("orderFilter", req.m_orderFilter.value());

    if (req.m_orderStatus.has_value())
        params.addQueryItem("orderStatus", EnumHelpers::orderStatusToString(req.m_orderStatus.value()));

    // Временной диапазон
    if (req.m_startTime.has_value())
        params.addQueryItem("startTime", QString::number(req.m_startTime.value()));

    if (req.m_endTime.has_value())
        params.addQueryItem("endTime", QString::number(req.m_endTime.value()));

    // Пагинация
    if (req.m_limit.has_value())
        params.addQueryItem("limit", QString::number(req.m_limit.value()));

    if (req.m_cursor.has_value() && !req.m_cursor->isEmpty())
        params.addQueryItem("cursor", req.m_cursor.value());

    return params;
}

void Core::Handlers::GetBybitOrderHistoryHandler::handle(const QJsonObject &data, const QVariant& requestContext, Service::PrivateServiceSignals *signaller)
{
    if (!data.contains("retCode") || data["retCode"].toInt() != 0)
    {
        emit signaller->errorOccurred(QString("Error processing request [endpoint = %1]: " + data["retMsg"].toString()).arg(endpoint()));
        return;
    }

    QJsonObject result = data["result"].toObject();
    QJsonArray list = result["list"].toArray();
    EnumHelpers::MarketType category = EnumHelpers::apiStringToMarketType(result["category"].toString());
    QString nextPageCursor = result["nextPageCursor"].toString();

    Orders orders{};

    for (const auto& item : std::as_const(list))
    {
        QJsonObject obj = item.toObject();
        OrderInfo o{};

        o.m_category = category;
        o.m_symbol = obj["symbol"].toString();
        o.m_orderId = obj["orderId"].toString();
        o.m_orderLinkId = obj["orderLinkId"].toString();
        o.m_side = EnumHelpers::stringToOrderSide(obj["side"].toString());
        o.m_orderType = EnumHelpers::stringToOrderType(obj["orderType"].toString());

        o.m_price = obj["price"].toString();
        o.m_qty = obj["qty"].toString();

        o.m_status = EnumHelpers::stringToOrderStatus(obj["orderStatus"].toString());
        o.m_positionIdx = obj.value("positionIdx").toInt(0);

        o.m_leavesQty = obj["leavesQty"].toString();
        o.m_cumExecQty = obj["cumExecQty"].toString();
        o.m_cumExecValue = obj["cumExecValue"].toString();
        o.m_cumExecFee = obj["cumExecFee"].toString();
        o.m_rejectReason = obj["rejectReason"].toString();

        o.m_createdTime = obj["createdTime"].toString().toLongLong();
        o.m_updatedTime = obj["updatedTime"].toString().toLongLong();

        orders.append(o);
    }

    emit signaller->orderHistoryPageReceived(orders, nextPageCursor, requestContext);
}
