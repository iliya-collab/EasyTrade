#pragma once
#include "StdEnums.hpp"
#include <QString>
#include <optional>

namespace Core::Tools
{

// Параметры запроса свечь
struct KlinesRequest
{
    MarketType m_category = MarketType::Unknown;
    QString m_symbol = "";
    Interval m_interval = Interval::Unknown;
    std::optional<qint64> m_start;
    std::optional<qint64> m_end;
    std::optional<int> m_limit = 200;
};

// ==========================================================================================
//  Параметры запросов управления ордерами
// ==========================================================================================

// Параметры создания нового ордера.
struct OrderRequest
{
    MarketType m_category = MarketType::Unknown; // Обязательно: spot, linear, inverse, option
    QString m_symbol = ""; // Обязательно: торговая пара, например "BTCUSDT"
    OrderSide m_side = OrderSide::Unknown; // Обязательно: Buy или Sell
    OrderType m_orderType = OrderType::Unknown; // Обязательно: Limit или Market
    QString m_qty = ""; // Обязательно: объем ордера (строкой, как того требует API)

    std::optional<QString> m_price; // Обязателен для Limit; для Market обычно не передается
    std::optional<QString> m_orderLinkId; // Опционально: клиентский ID для локального трекинга
    std::optional<TimeInForce> m_timeInForce = TimeInForce::GTC; // Опционально: GTC (по умолчанию), IOC, FOK, PostOnly
    std::optional<QString> m_takeProfit; // Опционально: цена тейк-профита
    std::optional<QString> m_stopLoss; // Опционально: цена стоп-лосса
    std::optional<QString> m_triggerPrice; // Опционально: триггерная цена для условных (стоп) ордеров
    std::optional<bool> m_reduceOnly = false; // Опционально: ордер только на закрытие позиции (для деривативов)
};

// Параметры изменения существующего ордера.
struct OrderAmendRequest
{
    MarketType m_category = MarketType::Unknown; // Обязательно
    QString m_symbol = ""; // Обязательно
    std::optional<QString> m_orderId; // Обязательно: orderId ИЛИ orderLinkId — нужен хотя бы один
    std::optional<QString> m_orderLinkId;

    std::optional<QString> m_qty; // Новое количество (опционально)
    std::optional<QString> m_price; // Новая цена (опционально)
    std::optional<QString> m_triggerPrice; // Новая триггерная цена для стоп-ордеров (опционально)
    std::optional<QString> m_takeProfit; // Новый уровень TP, "0" — удалить (опционально)
    std::optional<QString> m_stopLoss; // Новый уровень SL, "0" — удалить (опционально)
};

// Oтмена одного конкретного ордера.
struct OrderCancelRequest
{
    MarketType m_category = MarketType::Unknown; // Обязательно
    QString m_symbol = ""; // Обязательно
    std::optional<QString> m_orderId; // Обязательно: orderId ИЛИ orderLinkId — нужен хотя бы один
    std::optional<QString> m_orderLinkId;
};

// Массовая отмена ордеров.
// Для деривативов обязателен один из трех фильтров: symbol, baseCoin или settleCoin.
struct OrderCancelAllRequest
{
    MarketType m_category = MarketType::Unknown; // Обязательно

    std::optional<QString> m_symbol; // Высший приоритет фильтрации, если задан
    std::optional<QString> m_baseCoin; // Например "BTC"
    std::optional<QString> m_settleCoin; // Например "USDT"
};

// Запрос активных (и опционально недавних финальных) ордеров.
struct OpenOrdersRequest
{
    MarketType m_category = MarketType::Unknown; // Обязательно

    std::optional<QString> m_symbol; // Опционально: фильтр по паре
    std::optional<QString> m_orderId; // Опционально: поиск конкретного ордера
    std::optional<QString> m_orderLinkId;
    std::optional<int> m_openOnly = 0; // 0 — только открытые (по умолчанию); 1 — из последних 500 в финальном статусе
};

// Запрос истории закрытых/отмененных/исполненных ордеров (архив до 2 лет).
struct OrderHistoryRequest
{
    MarketType m_category = MarketType::Unknown; // Обязательно

    std::optional<QString> m_symbol; // Опционально: фильтр по паре
    std::optional<QString> m_orderId; // Опционально: поиск конкретного ордера
    std::optional<QString> m_orderLinkId;
    std::optional<qint64> m_startTime = 0; // Опционально: начало диапазона (мс)
    std::optional<qint64> m_endTime = 0; // Опционально: конец диапазона (мс)
    std::optional<int> m_limit = 50; // Опционально: записей на страницу (обычно до 50-100)
};

}

Q_DECLARE_METATYPE(Core::Tools::OrderRequest)
Q_DECLARE_METATYPE(Core::Tools::OrderAmendRequest)
Q_DECLARE_METATYPE(Core::Tools::OrderCancelRequest)
Q_DECLARE_METATYPE(Core::Tools::OrderCancelAllRequest)
Q_DECLARE_METATYPE(Core::Tools::OpenOrdersRequest)
Q_DECLARE_METATYPE(Core::Tools::OrderHistoryRequest)
