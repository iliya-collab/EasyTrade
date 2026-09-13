#include "GetBybitOpenOrdersHandler.hpp"

QUrlQuery Core::Markets::GetBybitOpenOrdersHandler::buildRequest(const Tools::OpenOrdersRequest &req)
{
    QUrlQuery params;
    params.addQueryItem("category", Tools::marketTypeToString(req.m_category));
    if (req.m_symbol.has_value() && !req.m_symbol->isEmpty())
        params.addQueryItem("symbol", req.m_symbol.value());
    if (req.m_orderId.has_value() && !req.m_orderId->isEmpty())
        params.addQueryItem("orderId", req.m_orderId.value());
    if (req.m_orderLinkId.has_value() && !req.m_orderLinkId->isEmpty())
        params.addQueryItem("orderLinkId", req.m_orderLinkId.value());
    if (req.m_openOnly.has_value())
        params.addQueryItem("openOnly", QString::number(req.m_openOnly.value()));
    return params;
}

void Core::Markets::GetBybitOpenOrdersHandler::handle(const QJsonObject &data, IPrivateService *service)
{
    if (!data.contains("retMsg") || data["retMsg"].toString() != "OK")
    {
        emit service->errorOccurred(QString("Error processing request [endpoint = %1]: " + data["retMsg"].toString()).arg(endpoint()));
        return;
    }

    QJsonObject result = data["result"].toObject();
    QJsonArray list = result["list"].toArray();
    Tools::MarketType category = Tools::stringToMarketType(result["category"].toString());

    QList<Tools::OrderInfo> orders{};

    for (const auto& item : std::as_const(list))
    {
        QJsonObject obj = item.toObject();
        Tools::OrderInfo o{};

        o.m_category = category;
        o.m_symbol = obj["symbol"].toString();
        o.m_orderId = obj["orderId"].toString();
        o.m_orderLinkId = obj["orderLinkId"].toString();
        o.m_side = Tools::stringToOrderSide(obj["side"].toString());
        o.m_orderType = Tools::stringToOrderType(obj["orderType"].toString());

        o.m_price = obj["price"].toString();
        o.m_qty = obj["qty"].toString();

        o.m_status = Tools::stringToOrderStatus(obj["orderStatus"].toString());
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

    emit service->openOrdersReceived(orders);
}
