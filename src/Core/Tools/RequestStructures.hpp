#pragma once
#include "Types/EnumHelpers.hpp"
#include <QString>
#include <optional>
using namespace Core::Tools::Types;

namespace Core::Tools
{

struct FeeRateRequest
{
    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown;
    QString m_symbol = "";
};

struct InstrumentsInfoRequest
{
    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown;
    std::optional<int> m_limit = 500;
    std::optional<QString> m_cursor;
};

// Параметры запроса свечь
struct KlinesRequest
{
    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown;
    QString m_symbol = "";
    EnumHelpers::Interval m_interval = EnumHelpers::Interval::Unknown;
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
    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown; // Обязательно: spot, linear, inverse, option
    QString m_symbol = ""; // Обязательно: торговая пара, например "BTCUSDT"
    EnumHelpers::OrderSide m_side = EnumHelpers::OrderSide::Unknown; // Обязательно: Buy или Sell
    EnumHelpers::OrderType m_orderType = EnumHelpers::OrderType::Unknown; // Обязательно: Limit или Market
    QString m_qty = ""; // Обязательно: объем ордера (строкой, как того требует API)

    std::optional<QString> m_price; // Обязателен для Limit; для Market обычно не передается
    std::optional<QString> m_orderLinkId; // Опционально: клиентский ID для локального трекинга
    std::optional<EnumHelpers::TimeInForce> m_timeInForce = EnumHelpers::TimeInForce::GTC; // Опционально: GTC (по умолчанию), IOC, FOK, PostOnly
    std::optional<QString> m_takeProfit; // Опционально: цена тейк-профита
    std::optional<QString> m_stopLoss; // Опционально: цена стоп-лосса
    std::optional<QString> m_triggerPrice; // Опционально: триггерная цена для условных (стоп) ордеров
    std::optional<bool> m_reduceOnly = false; // Опционально: ордер только на закрытие позиции (для деривативов)
};

// Параметры изменения существующего ордера.
struct OrderAmendRequest
{
    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown; // Обязательно
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
    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown; // Обязательно
    QString m_symbol = ""; // Обязательно
    std::optional<QString> m_orderId; // Обязательно: orderId ИЛИ orderLinkId — нужен хотя бы один
    std::optional<QString> m_orderLinkId;
};

// Массовая отмена ордеров.
// Для деривативов обязателен один из трех фильтров: symbol, baseCoin или settleCoin.
struct OrderCancelAllRequest
{
    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown; // Обязательно

    std::optional<QString> m_symbol; // Высший приоритет фильтрации, если задан
    std::optional<QString> m_baseCoin; // Например "BTC"
    std::optional<QString> m_settleCoin; // Например "USDT"
};

// Запрос активных (и опционально недавних финальных) ордеров.
struct OpenOrdersRequest
{
    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown; // Обязательно
    std::optional<QString> m_symbol; // Опционально: фильтр по паре
    std::optional<QString> m_baseCoin;    // Базовая монета (только для опционов)
    std::optional<QString> m_settleCoin;  // Валюта расчета (для деривативов)
    std::optional<QString> m_orderId; // Опционально: поиск конкретного ордера
    std::optional<QString> m_orderLinkId;
    std::optional<int> m_openOnly = 0;  // 0 — только открытые (по умолчанию); 1 — из последних 500 в финальном статусе
    std::optional<QString> m_orderFilter;
    std::optional<int> m_limit = 20;
    std::optional<QString> m_cursor;       // Курсор для получения следующей страницы (nextPageCursor)
};

// Запрос истории закрытых/отмененных/исполненных ордеров (архив до 2 лет).
struct OrderHistoryRequest
{
    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown; // Обязательно
    std::optional<QString> m_symbol; // Опционально: фильтр по паре
    std::optional<QString> m_baseCoin;    // Базовая монета (например, BTC)
    std::optional<QString> m_settleCoin;  // Валюта расчета
    std::optional<QString> m_orderId; // Опционально: поиск конкретного ордера
    std::optional<QString> m_orderLinkId;
    std::optional<QString> m_orderFilter; // Фильтр типа ордера (Order, StopOrder, tpslOrder...)
    std::optional<EnumHelpers::OrderStatus> m_orderStatus;
    std::optional<qint64> m_startTime; // Опционально: начало диапазона (мс)
    std::optional<qint64> m_endTime; // Опционально: конец диапазона (мс)
    std::optional<int> m_limit = 20; // Опционально: записей на страницу
    std::optional<QString> m_cursor;       // Курсор для получения следующей страницы (nextPageCursor)
};

// История сделок
struct TradeHistoryRequest
{
    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown;
    std::optional<QString> m_symbol;       // Фильтр по торговой паре (например, "BTCUSDT")
    std::optional<QString> m_orderId;      // Поиск по ID ордера
    std::optional<QString> m_orderLinkId;  // Поиск по клиентскому ID ордера
    std::optional<QString> m_baseCoin;     // Базовая монета
    std::optional<QString> m_settleCoin;   // Монета расчета
    std::optional<QString> m_execType;     // Тип исполнения (Trade, Adl, Funding, etc.)
    std::optional<qint64> m_startTime;     // Начало диапазона
    std::optional<qint64> m_endTime;       // Конец диапазона
    std::optional<int> m_limit = 50;       // Записей на страницу
    std::optional<QString> m_cursor;       // Курсор для получения следующей страницы (nextPageCursor)
};

}

Q_DECLARE_METATYPE(Core::Tools::InstrumentsInfoRequest)
Q_DECLARE_METATYPE(Core::Tools::KlinesRequest)
Q_DECLARE_METATYPE(Core::Tools::OrderRequest)
Q_DECLARE_METATYPE(Core::Tools::OrderAmendRequest)
Q_DECLARE_METATYPE(Core::Tools::OrderCancelRequest)
Q_DECLARE_METATYPE(Core::Tools::OrderCancelAllRequest)
Q_DECLARE_METATYPE(Core::Tools::OpenOrdersRequest)
Q_DECLARE_METATYPE(Core::Tools::OrderHistoryRequest)
Q_DECLARE_METATYPE(Core::Tools::TradeHistoryRequest)
