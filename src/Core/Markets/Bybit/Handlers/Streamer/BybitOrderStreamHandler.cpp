#include "BybitOrderStreamHandler.hpp"
#include "Tools/Logging.hpp"

void Core::Handlers::BybitOrderStreamHandler::handle(const QString& id, const QJsonObject &data, Streamer::PrivateStreamerSignals *signaller)
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

        Tools::OrderInfo order{};

        order.m_category    = Tools::EnumHelpers::apiStringToMarketType(dataObj["category"].toString());
        order.m_orderId     = dataObj["orderId"].toString();
        order.m_orderLinkId = dataObj["orderLinkId"].toString();
        order.m_symbol      = dataObj["symbol"].toString();
        order.m_orderType   = Tools::EnumHelpers::stringToOrderType(dataObj["orderType"].toString());

        QString strSide = dataObj["side"].toString();
        if (strSide == "Buy")
            order.m_side = Tools::EnumHelpers::OrderSide::Buy;
        else if (strSide == "Sell")
            order.m_side = Tools::EnumHelpers::OrderSide::Sell;
        else
            order.m_side = Tools::EnumHelpers::OrderSide::Unknown;

        QString strStatus = dataObj["orderStatus"].toString();
        if (strStatus == "New")
            order.m_status = Tools::EnumHelpers::OrderStatus::New;
        else if (strStatus == "PartiallyFilled")
            order.m_status = Tools::EnumHelpers::OrderStatus::PartiallyFilled;
        else if (strStatus == "Filled")
            order.m_status = Tools::EnumHelpers::OrderStatus::Filled;
        else if (strStatus == "Cancelled" || strStatus == "Deactivated")
            order.m_status = Tools::EnumHelpers::OrderStatus::Cancelled;
        else if (strStatus == "Rejected")
            order.m_status = Tools::EnumHelpers::OrderStatus::Rejected;
        else
            order.m_status = Tools::EnumHelpers::OrderStatus::Unknown;

        order.m_price        = dataObj["price"].toString();
        order.m_qty          = dataObj["qty"].toString();
        order.m_leavesQty    = dataObj["leavesQty"].toString();
        order.m_cumExecQty   = dataObj["cumExecQty"].toString();
        order.m_cumExecValue = dataObj["cumExecValue"].toString();
        order.m_cumExecFee   = dataObj["cumExecFee"].toString();

        order.m_rejectReason = dataObj["rejectReason"].toString();
        order.m_createdTime  = dataObj["createdTime"].toString().toLongLong();
        order.m_updatedTime  = dataObj["updatedTime"].toString().toLongLong();

        order.m_positionIdx  = dataObj["positionIdx"].toVariant().toInt();

        emit signaller->orderUpdated(order);
    }
}
