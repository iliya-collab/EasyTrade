#include "PostBybitCancelAllOrdersHandler.hpp"

QByteArray Core::Handlers::PostBybitCancelAllOrdersHandler::buildRequestBody(const Tools::OrderCancelAllRequest &req)
{
    QJsonObject body;

    body["category"] = Tools::EnumHelpers::marketTypeToApiString(req.m_category);

    if (req.m_symbol.has_value() && !req.m_symbol->isEmpty())
        body["symbol"] = req.m_symbol.value();
    if (req.m_baseCoin.has_value() && !req.m_baseCoin->isEmpty())
        body["baseCoin"] = req.m_baseCoin.value();
    if (req.m_settleCoin.has_value() && !req.m_settleCoin->isEmpty())
        body["settleCoin"] = req.m_settleCoin.value();

    return QJsonDocument(body).toJson(QJsonDocument::Compact);
}

void Core::Handlers::PostBybitCancelAllOrdersHandler::handle(const QJsonObject &data, const QVariant& requestContext, Service::PrivateServiceSignals *signaller)
{
    if (!data.contains("retCode") || data["retCode"].toInt() != 0)
    {
        emit signaller->errorOccurred(QString("Error processing request [endpoint = %1]: " + data["retMsg"].toString()).arg(endpoint()));
        return;
    }

    QJsonObject result = data["result"].toObject();
    QJsonArray list = result["list"].toArray();

    QStringList lstIds{};
    for (const auto& item : std::as_const(list))
    {
        QJsonObject itemObj = item.toObject();
        QString orderId = itemObj["orderId"].toString();
        lstIds.append(orderId);
    }

    emit signaller->allOrdersCancelled(lstIds);
}
