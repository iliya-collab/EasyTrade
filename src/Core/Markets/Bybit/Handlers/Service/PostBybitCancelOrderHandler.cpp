#include "PostBybitCancelOrderHandler.hpp"

QByteArray Core::Handlers::PostBybitCancelOrderHandler::buildRequestBody(const Tools::OrderCancelRequest &req)
{
    QJsonObject body;

    body["category"] = EnumHelpers::marketTypeToApiString(req.m_category);
    body["symbol"] = req.m_symbol;

    if (req.m_orderLinkId.has_value() && !req.m_orderLinkId->isEmpty())
        body["orderLinkId"] = req.m_orderLinkId.value();
    if (req.m_orderId.has_value() && !req.m_orderId->isEmpty())
        body["orderId"] = req.m_orderId.value();

    return QJsonDocument(body).toJson(QJsonDocument::Compact);
}

void Core::Handlers::PostBybitCancelOrderHandler::handle(const QJsonObject &data, const QVariant& requestContext, Service::PrivateServiceSignals *signaller)
{
    if (!data.contains("retCode") || data["retCode"].toInt() != 0)
    {
        emit signaller->errorOccurred(QString("Error processing request [endpoint = %1]: " + data["retMsg"].toString()).arg(endpoint()));
        return;
    }

    QJsonObject result = data["result"].toObject();
    QString orderId = result["orderId"].toString();
    QString orderLinkId = result["orderLinkId"].toString();

    emit signaller->orderCancelled(orderId, orderLinkId);
}
