#include "GetBybitTradeHistoryHandler.hpp"

QUrlQuery Core::Handlers::GetBybitTradeHistoryHandler::buildRequest(const Tools::TradeHistoryRequest &req)
{
    QUrlQuery params;

    // Обязательный параметр
    params.addQueryItem("category", Tools::EnumHelpers::marketTypeToApiString(req.m_category));

    // Фильтры по инструменту
    if (req.m_symbol.has_value() && !req.m_symbol->isEmpty())
        params.addQueryItem("symbol", req.m_symbol.value());

    if (req.m_baseCoin.has_value() && !req.m_baseCoin->isEmpty())
        params.addQueryItem("baseCoin", req.m_baseCoin.value());

    if (req.m_settleCoin.has_value() && !req.m_settleCoin->isEmpty())
        params.addQueryItem("settleCoin", req.m_settleCoin.value());

    // Идентификаторы ордера. Имеют приоритет: если orderId/orderLinkId переданы,
    // Bybit игнорирует остальные фильтры (symbol, baseCoin и т.д.)
    if (req.m_orderId.has_value() && !req.m_orderId->isEmpty())
        params.addQueryItem("orderId", req.m_orderId.value());

    if (req.m_orderLinkId.has_value() && !req.m_orderLinkId->isEmpty())
        params.addQueryItem("orderLinkId", req.m_orderLinkId.value());

    // Тип исполнения (Trade, Funding, ...)
    if (req.m_execType.has_value() && !req.m_execType->isEmpty())
        params.addQueryItem("execType", req.m_execType.value());

    // Временной диапазон (между startTime и endTime не больше 7 дней)
    if (req.m_startTime.has_value())
        params.addQueryItem("startTime", QString::number(req.m_startTime.value()));

    if (req.m_endTime.has_value())
        params.addQueryItem("endTime", QString::number(req.m_endTime.value()));

    // Пагинация (limit: 1..100, по умолчанию 50)
    if (req.m_limit.has_value())
        params.addQueryItem("limit", QString::number(req.m_limit.value()));

    if (req.m_cursor.has_value() && !req.m_cursor->isEmpty())
        params.addQueryItem("cursor", req.m_cursor.value());

    return params;

}

void Core::Handlers::GetBybitTradeHistoryHandler::handle(const QJsonObject &data, const QVariant &requestContext, Service::PrivateServiceSignals *signaller)
{
    const QJsonObject result = data.value(QStringLiteral("result")).toObject();
    const QJsonArray list = result.value(QStringLiteral("list")).toArray();

    Tools::EnumHelpers::MarketType category = Tools::EnumHelpers::apiStringToMarketType(
        result.value(QStringLiteral("category")).toString());

    QList<Tools::ExecutionInfo> executions;
    executions.reserve(list.size());


    for (const QJsonValue &item : list)
    {
        const QJsonObject obj = item.toObject();

        Tools::ExecutionInfo info;

        info.m_category = category;

        info.m_symbol      = obj.value(QStringLiteral("symbol")).toString();
        info.m_execId      = obj.value(QStringLiteral("execId")).toString();
        info.m_orderId     = obj.value(QStringLiteral("orderId")).toString();
        info.m_orderLinkId = obj.value(QStringLiteral("orderLinkId")).toString();

        info.m_side = Tools::EnumHelpers::stringToOrderSide(obj.value(QStringLiteral("side")).toString());

        info.m_execPrice = obj.value(QStringLiteral("execPrice")).toString();
        info.m_execQty   = obj.value(QStringLiteral("execQty")).toString();
        info.m_execValue = obj.value(QStringLiteral("execValue")).toString();

        info.m_execType = Tools::EnumHelpers::stringToExecType(obj.value(QStringLiteral("execType")).toString());

        info.m_execFee     = obj.value(QStringLiteral("execFee")).toString();
        info.m_feeCurrency = obj.value(QStringLiteral("feeCurrency")).toString();

        info.m_isMaker  = obj.value(QStringLiteral("isMaker")).toBool(false);
        info.m_execTime = obj.value(QStringLiteral("execTime")).toString().toLongLong();

        executions.append(info);
    }

    if (signaller)
        emit signaller->tradeHistoryPageReceived(executions, result.value(QStringLiteral("nextPageCursor")).toString(), requestContext);
}
