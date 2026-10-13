#pragma once
#include "QueryStructures.hpp"
#include <QVariantMap>
#include <optional>
#include <expected>

namespace Core::Tools
{

// ==========================================================================================
//  Сборка структур-запросов из QVariantMap (JS-объекта, пришедшего из QML) с валидацией.
// ==========================================================================================

    namespace detail
    {

        inline std::optional<QString> optString(const QVariantMap& map, const QString& key)
        {
            auto it = map.constFind(key);
            if (it == map.constEnd())
                return std::nullopt;
            auto res = it->toString();
            return res.isEmpty() ? std::nullopt : std::make_optional(res);
        }

        inline std::optional<bool> optBool(const QVariantMap& map, const QString& key)
        {
            auto it = map.constFind(key);
            if (it == map.constEnd())
                return std::nullopt;
            return it->toBool();
        }

        inline std::optional<qint64> optInt64(const QVariantMap& map, const QString& key)
        {
            auto it = map.constFind(key);
            if (it == map.constEnd())
                return std::nullopt;
            return it->toLongLong();
        }

        inline QString reqString(const QVariantMap& map, const QString& key)
        {
            return map.value(key).toString();
        }

        inline bool hasEitherId(std::optional<QString> orderId, std::optional<QString> orderLinkId)
        {
            return (orderId.has_value() && !orderId->isEmpty()) ||
                   (orderLinkId.has_value() && !orderLinkId->isEmpty());
        }

    }

    inline std::expected<KlinesRequest, QString> buildKlinesRequest(const QVariantMap& params)
    {
        KlinesRequest req;

        // Обязательные
        req.m_category = EnumHelpers::apiStringToMarketType(detail::reqString(params, "category"));
        req.m_symbol = detail::reqString(params, "symbol");
        req.m_interval = EnumHelpers::stringToInterval(detail::reqString(params, "interval"));

        // Опциональные
        req.m_start = detail::optInt64(params, "start");
        req.m_end = detail::optInt64(params, "end");
        if (params.contains("limit"))
            req.m_limit = detail::optInt64(params, "limit");

        // Обработка
        if (req.m_category == EnumHelpers::MarketType::Unknown)
            return std::unexpected<QString>("requestKlines: Invalid \'category\' parameter");
        if (req.m_symbol.isEmpty())
            return std::unexpected<QString>("requestKlines: The \'symbol\' parameter must not be empty");
        if (req.m_interval == EnumHelpers::Interval::Unknown)
            return std::unexpected<QString>("requestKlines: Invalid \'interval\' parameter");
        if (req.m_start.has_value() &&
            req.m_end.has_value() &&
            *req.m_start > *req.m_end)
            return std::unexpected("requestKlines: \'start\' cannot be later than \'end\'");
        if (req.m_limit.has_value() &&
            ((*req.m_limit <= 0) || (*req.m_limit > 1000)))
            return std::unexpected("requestKlines: \'limit\' cannot be later than 0 or more than 1000");

        return req;
    }

    inline std::expected<OrderRequest, QString> buildOrderRequest(const QVariantMap& params)
    {
        OrderRequest req;

        // Обязательные
        req.m_category = EnumHelpers::apiStringToMarketType(detail::reqString(params, "category"));
        req.m_symbol = detail::reqString(params, "symbol");
        req.m_side = EnumHelpers::stringToOrderSide(detail::reqString(params, "side"));
        req.m_orderType = EnumHelpers::stringToOrderType(detail::reqString(params, "orderType"));
        req.m_qty = detail::reqString(params, "qty");

        // Опциональные
        req.m_price = detail::optString(params, "price");
        req.m_orderLinkId = detail::optString(params, "orderLinkId");
        req.m_takeProfit = detail::optString(params, "takeProfit");
        req.m_stopLoss = detail::optString(params, "stopLoss");
        req.m_triggerPrice = detail::optString(params, "triggerPrice");
        req.m_reduceOnly = detail::optBool(params, "reduceOnly");
        if (params.contains("timeInForce"))
            req.m_timeInForce = EnumHelpers::stringToTimeInForce(detail::reqString(params, "timeInForce"));

        // Обработка
        if (req.m_category == EnumHelpers::MarketType::Unknown)
            return std::unexpected<QString>("createOrder: Invalid \'category\' parameter");
        if (req.m_symbol.isEmpty())
            return std::unexpected<QString>("createOrder: The \'symbol\' parameter must not be empty");
        if (req.m_orderType == EnumHelpers::OrderType::Unknown)
            return std::unexpected<QString>("createOrder: Invalid \'orderType\' parameter");
        if (req.m_side == EnumHelpers::OrderSide::Unknown)
            return std::unexpected<QString>("createOrder: Invalid \'side\' parameter");
        if (req.m_qty.isEmpty())
            return std::unexpected<QString>("createOrder: The \'qty\' parameter must not be empty");
        if (req.m_orderType == EnumHelpers::OrderType::Limit && (!req.m_price.has_value() || req.m_price->isEmpty()))
            return std::unexpected<QString>("createOrder: The \'price\' parameter must not be empty for a limit order");

        return req;
    }

    inline std::expected<OrderAmendRequest, QString> buildOrderAmendRequest(const QVariantMap& params)
    {
        OrderAmendRequest req;

        // Обязательные
        req.m_category = EnumHelpers::apiStringToMarketType(detail::reqString(params, "category"));
        req.m_symbol = detail::reqString(params, "symbol");
        req.m_orderId = detail::optString(params, "orderId");
        req.m_orderLinkId = detail::optString(params, "orderLinkId");

        // Опциональные
        req.m_qty = detail::optString(params, "qty");
        req.m_price = detail::optString(params, "price");
        req.m_takeProfit = detail::optString(params, "takeProfit");
        req.m_stopLoss = detail::optString(params, "stopLoss");
        req.m_triggerPrice = detail::optString(params, "triggerPrice");

        // Обработка
        if (req.m_category == EnumHelpers::MarketType::Unknown)
            return std::unexpected<QString>("amendOrder: Invalid \'category\' parameter");
        if (req.m_symbol.isEmpty())
            return std::unexpected<QString>("amendOrder: The \'symbol\' parameter must not be empty");
        if (!detail::hasEitherId(req.m_orderId, req.m_orderLinkId))
            return std::unexpected<QString>("amendOrder: One of the parameters 'orderId' or 'orderLinkId' must be specified");

        return req;
    }

    inline std::expected<OrderCancelRequest, QString> buildOrderCancelRequest(const QVariantMap& params)
    {
        OrderCancelRequest req;

        // Обязательные
        req.m_category = EnumHelpers::apiStringToMarketType(detail::reqString(params, "category"));
        req.m_symbol = detail::reqString(params, "symbol");
        req.m_orderId = detail::optString(params, "orderId");
        req.m_orderLinkId = detail::optString(params, "orderLinkId");

        // Обработка
        if (req.m_category == EnumHelpers::MarketType::Unknown)
            return std::unexpected<QString>("cancelOrder: Invalid \'category\' parameter");
        if (req.m_symbol.isEmpty())
            return std::unexpected<QString>("cancelOrder: The \'symbol\' parameter must not be empty");
        if (!detail::hasEitherId(req.m_orderId, req.m_orderLinkId))
            return std::unexpected<QString>("cancelOrder: One of the parameters 'orderId' or 'orderLinkId' must be specified");

        return req;
    }

    inline std::expected<OrderCancelAllRequest, QString> buildOrderCancelAllRequest(const QVariantMap& params)
    {
        OrderCancelAllRequest req;

        // Обязательные
        req.m_category = EnumHelpers::apiStringToMarketType(detail::reqString(params, "category"));

        // Опциональные
        req.m_symbol = detail::optString(params, "symbol");
        req.m_baseCoin = detail::optString(params, "baseCoin");
        req.m_settleCoin = detail::optString(params, "settleCoin");

        const bool isDerivative = (req.m_category == EnumHelpers::MarketType::Linear || req.m_category == EnumHelpers::MarketType::Inverse);
        const bool hasFilter = (req.m_symbol.has_value() && !req.m_symbol->isEmpty()) ||
                               (req.m_baseCoin.has_value() && !req.m_baseCoin->isEmpty()) ||
                               (req.m_settleCoin.has_value() && !req.m_settleCoin->isEmpty());

        // Обработка
        if (isDerivative && !hasFilter)
            return std::unexpected<QString>("cancelAllOrders: For linear/inverse, a \'symbol\', \'baseCoin\', or \'settleCoin\' is required");

        return req;
    }

    inline std::expected<OpenOrdersRequest, QString> buildOpenOrdersRequest(const QVariantMap& params)
    {
        OpenOrdersRequest req;

        req.m_category = EnumHelpers::apiStringToMarketType(detail::reqString(params, "category"));
        if (req.m_category == EnumHelpers::MarketType::Unknown)
            return std::unexpected<QString>("requestOpenOrders: Invalid 'category' parameter");

        // Для категорий linear и inverse параметр symbol является ОБЯЗАТЕЛЬНЫМ в Bybit API V5.
        // На споте (spot) его можно не передавать.
        auto symbolStr = detail::optString(params, "symbol");
        if (symbolStr.has_value())
        {
            if ((req.m_category == EnumHelpers::MarketType::Linear || req.m_category == EnumHelpers::MarketType::Inverse) && symbolStr->isEmpty())
                return std::unexpected<QString>("requestOpenOrders: 'symbol' parameter is required for linear/inverse categories");
        }
        req.m_symbol = symbolStr;

        req.m_baseCoin    = detail::optString(params, "baseCoin");
        req.m_settleCoin  = detail::optString(params, "settleCoin");
        req.m_orderId     = detail::optString(params, "orderId");
        req.m_orderLinkId = detail::optString(params, "orderLinkId");
        req.m_orderFilter = detail::optString(params, "orderFilter");

        auto openOnlyOpt = detail::optBool(params, "openOnly");
        if (openOnlyOpt.has_value())
            req.m_openOnly = *openOnlyOpt ? 1 : 0;

        auto limitOpt = detail::optInt64(params, "limit");
        if (limitOpt.has_value())
            req.m_limit = *limitOpt;

        auto cursorOpt = detail::optString(params, "cursor");
        if (cursorOpt.has_value() && !cursorOpt->isEmpty())
            req.m_cursor = cursorOpt;

        return req;
    }

    inline std::expected<OrderHistoryRequest, QString> buildOrderHistoryRequest(const QVariantMap& params)
    {
        OrderHistoryRequest req;

        req.m_category = EnumHelpers::apiStringToMarketType(detail::reqString(params, "category"));
        if (req.m_category == EnumHelpers::MarketType::Unknown)
            return std::unexpected<QString>("requestOrderHistory: Invalid 'category' parameter");


        req.m_symbol      = detail::optString(params, "symbol");
        req.m_baseCoin    = detail::optString(params, "baseCoin");
        req.m_settleCoin  = detail::optString(params, "settleCoin");
        req.m_orderId     = detail::optString(params, "orderId");
        req.m_orderLinkId = detail::optString(params, "orderLinkId");
        req.m_orderFilter = detail::optString(params, "orderFilter");

        if (params.contains("orderStatus"))
            req.m_orderStatus = EnumHelpers::stringToOrderStatus(detail::reqString(params, "orderStatus"));

        req.m_startTime = detail::optInt64(params, "startTime");
        req.m_endTime   = detail::optInt64(params, "endTime");

        if (req.m_startTime.has_value() && req.m_endTime.has_value() &&
            (*req.m_startTime > *req.m_endTime))
                return std::unexpected<QString>("requestOrderHistory: 'startTime' cannot be later than 'endTime'");

        auto limitOpt = detail::optInt64(params, "limit");
        if (limitOpt.has_value())
            req.m_limit = *limitOpt;

        auto cursorOpt = detail::optString(params, "cursor");
        if (cursorOpt.has_value() && !cursorOpt->isEmpty())
            req.m_cursor = cursorOpt;

        return req;
    }

    inline std::expected<TradeHistoryRequest, QString> buildTradeHistoryRequest(const QVariantMap& params)
    {
        TradeHistoryRequest req;

        req.m_category = EnumHelpers::apiStringToMarketType(detail::reqString(params, "category"));
        if (req.m_category == EnumHelpers::MarketType::Unknown)
            return std::unexpected<QString>("requestTradeHistory: Invalid 'category' parameter");


        req.m_symbol      = detail::optString(params, "symbol");
        req.m_orderId     = detail::optString(params, "orderId");
        req.m_orderLinkId = detail::optString(params, "orderLinkId");
        req.m_baseCoin    = detail::optString(params, "baseCoin");
        req.m_settleCoin  = detail::optString(params, "settleCoin");
        req.m_execType    = detail::optString(params, "execType");

        req.m_startTime = detail::optInt64(params, "startTime");
        req.m_endTime   = detail::optInt64(params, "endTime");

        if (req.m_startTime.has_value() && req.m_endTime.has_value())
        {
            if (*req.m_startTime > *req.m_endTime)
                return std::unexpected<QString>("requestTradeHistory: 'startTime' cannot be later than 'endTime'");


            qint64 sevenDaysMs = 7LL * 24 * 60 * 60 * 1000;
            if (*req.m_endTime - *req.m_startTime > sevenDaysMs)
                return std::unexpected<QString>("requestTradeHistory: The separation between startTime and endTime cannot be more than 7 days");

        }

        auto limitOpt = detail::optInt64(params, "limit");
        if (limitOpt.has_value())
            req.m_limit = *limitOpt;

        auto cursorOpt = detail::optString(params, "cursor");
        if (cursorOpt.has_value() && !cursorOpt->isEmpty())
            req.m_cursor = cursorOpt;

        return req;
    }

}