#pragma once
#include "Export/ExportFormats.hpp"
#include <QString>
#include <qobject.h>

namespace Core::Tools
{
Q_NAMESPACE

enum class Interval
{
    Min1,
    Min3,
    Min5,
    Min15,
    Min30,
    Hour1,
    Hour2,
    Hour4,
    Hour6,
    Hour12,
    Day,
    Week,
    Month,
    Unknown
};
Q_ENUM_NS(Interval)

// ==========================================================================================
//  Тип рынка / категория инструмента
// ==========================================================================================

enum class MarketType
{
    Spot,
    Linear,
    Inverse,
    Option,
    Unknown
};
Q_ENUM_NS(MarketType)

// ==========================================================================================
//  Направление / статус ордера / тип исполнения
// ==========================================================================================

// Направление операции
enum class OrderSide
{
    Buy, // Покупка. Для спота — приобретение базового актива. Для фьючерсов — открытие Long-позиции или закрытие Short-позиции.
    Sell, // Продажа. Для спота — продажа актива. Для фьючерсов — открытие Short-позиции или закрытие Long-позиции.
    Unknown // Неизвестное направление. Защитное значение на случай десериализации некорректных данных или при полном закрытии позиции (когда Bybit присылает side: "").
};
Q_ENUM_NS(OrderSide)

// Тип ордера при выставлении
enum class OrderType
{
    Limit, // Лимитный ордер — исполняется по указанной цене (m_price обязателен).
    Market, // Рыночный ордер — исполняется по текущей рыночной цене немедленно.
    Unknown
};
Q_ENUM_NS(OrderType)

enum class OrderStatus
{
    New, // Ордер успешно принят торговым движком биржи и размещен в стакане (активен).
    PartiallyFilled, // Ордер частично исполнен. Часть объема уже превратилась в сделки, но остаток (m_leavesQty) всё еще находится в стакане.
    Filled, // Ордер полностью исполнен. Весь заявленный объем (m_qty) был сведен со встречными ордерами. Ордер неактивен.
    Cancelled, // Ордер отменен пользователем или системой (например, по истечении времени действия или по правилу Post-Only), до того как был полностью исполнен.
    Rejected, // Ордер отклонен биржей на этапе валидации (например, не хватило маржи, неверная цена, сработал риск-лимит) и не попал в стакан.
    Unknown // Неизвестный статус (или редкие специфичные статусы Bybit вроде Deactivated, если они не смаппированы отдельно).
};
Q_ENUM_NS(OrderStatus)

enum class ExecType
{
    Trade, // Обычная торговая сделка на бирже (ордер сопоставился со встречным ордером в стакане).
    Adl, // Авто-делеверидж (Auto-Deleveraging). Принудительное закрытие вашей прибыльной позиции встречным ордером ликвидируемого игрока при нехватке страхового фонда биржи.
    Funding, // Периодическое списание или начисление комиссии за удержание позиций на бессрочных фьючерсах (обычно каждые 4-8 часов).
    Liquidation, // Сделка, совершенная в процессе принудительной ликвидации позиции из-за нехватки поддерживающей маржи.
    Unknown // Неизвестный тип исполнения.
};
Q_ENUM_NS(ExecType)

// Время действия ордера
enum class TimeInForce
{
    GTC, // Good Till Cancel — действует, пока не будет исполнен или отменен вручную (по умолчанию).
    IOC, // Immediate Or Cancel — исполняется немедленно (полностью или частично), неисполненный остаток отменяется.
    FOK, // Fill Or Kill — исполняется полностью и немедленно, либо отменяется целиком.
    PostOnly, // Только мейкер — ордер отклоняется, если исполнился бы сразу как тейкер.
    Unknown
};
Q_ENUM_NS(TimeInForce)

// ---- Конвертация enum <-> строка для REST-запросов и парсинга ответов ----

inline QString intervalToString(Interval interval)
{
    switch (interval)
    {
    case Interval::Min1:    return "1";
    case Interval::Min3:    return "3";
    case Interval::Min5:    return "5";
    case Interval::Min15:   return "15";
    case Interval::Min30:   return "30";
    case Interval::Hour1:   return "60";
    case Interval::Hour2:   return "120";
    case Interval::Hour4:   return "240";
    case Interval::Hour6:   return "360";
    case Interval::Hour12:  return "720";
    case Interval::Day:     return "D";
    case Interval::Week:    return "W";
    case Interval::Month:   return "M";
    default:                return "";
    }
}

inline Interval stringToInterval(const QString &intervalStr)
{
    if (intervalStr == "1")   return Interval::Min1;
    if (intervalStr == "3")   return Interval::Min3;
    if (intervalStr == "5")   return Interval::Min5;
    if (intervalStr == "15")  return Interval::Min15;
    if (intervalStr == "30")  return Interval::Min30;

    if (intervalStr == "60")  return Interval::Hour1;
    if (intervalStr == "120") return Interval::Hour2;
    if (intervalStr == "240") return Interval::Hour4;
    if (intervalStr == "360") return Interval::Hour6;
    if (intervalStr == "720") return Interval::Hour12;

    if (intervalStr == "D")   return Interval::Day;
    if (intervalStr == "W")   return Interval::Week;
    if (intervalStr == "M")   return Interval::Month;

    return Interval::Unknown;
}

inline QString marketTypeToString(MarketType type)
{
    switch (type)
    {
    case MarketType::Spot:      return "spot";
    case MarketType::Linear:    return "linear";
    case MarketType::Inverse:   return "inverse";
    case MarketType::Option:    return "option";
    default:                    return "";
    }
}

inline MarketType stringToMarketType(const QString& type)
{
    if (type == "spot")     return MarketType::Spot;
    if (type == "linear")   return MarketType::Linear;
    if (type == "inverse")  return MarketType::Inverse;
    if (type == "option")   return MarketType::Option;

    return MarketType::Unknown;
}

inline QString orderSideToString(OrderSide side)
{
    switch (side)
    {
    case OrderSide::Buy:    return "Buy";
    case OrderSide::Sell:   return "Sell";
    default:                return "";
    }
}

inline OrderSide stringToOrderSide(const QString& side)
{
    if (side == "Buy")  return OrderSide::Buy;
    if (side == "Sell") return OrderSide::Sell;

    return OrderSide::Unknown;
}

inline QString orderStatusToString(OrderStatus status)
{
    switch (status)
    {
    case OrderStatus::New:              return "New";
    case OrderStatus::PartiallyFilled:  return "PartiallyFilled";
    case OrderStatus::Filled:           return "Filled";
    case OrderStatus::Cancelled:        return "Cancelled";
    case OrderStatus::Rejected:         return "Rejected";
    default:                            return "";
    }
}

inline OrderStatus stringToOrderStatus(const QString& status)
{
    if (status == "New")                return OrderStatus::New;
    if (status == "PartiallyFilled")    return OrderStatus::PartiallyFilled;
    if (status == "Filled")             return OrderStatus::Filled;
    if (status == "Cancelled")          return OrderStatus::Cancelled;
    if (status == "Rejected")           return OrderStatus::Rejected;\

    return OrderStatus::Unknown;
}

inline QString execTypeToString(ExecType type)
{
    switch (type)
    {
    case ExecType::Trade:       return "Trade";
    case ExecType::Adl:         return "Adl";
    case ExecType::Funding:     return "Funding";
    case ExecType::Liquidation: return "Liquidation";
    default:                    return "";
    }
}

inline ExecType stringToExecType(const QString& type)
{
    if (type == "Trade")        return ExecType::Trade;
    if (type == "Adl")          return ExecType::Adl;
    if (type == "Funding")      return ExecType::Funding;
    if (type == "Liquidation")  return ExecType::Liquidation;

    return ExecType::Unknown;
}

inline QString orderTypeToString(OrderType type)
{
    switch (type)
    {
    case OrderType::Limit:  return "Limit";
    case OrderType::Market: return "Market";
    default:                return "";
    }
}

inline OrderType stringToOrderType(const QString& type)
{
    if (type == "Limit")    return OrderType::Limit;
    if (type == "Market")   return OrderType::Market;

    return OrderType::Unknown;
}

inline QString timeInForceToString(TimeInForce tif)
{
    switch (tif)
    {
    case TimeInForce::GTC:      return "GTC";
    case TimeInForce::IOC:      return "IOC";
    case TimeInForce::FOK:      return "FOK";
    case TimeInForce::PostOnly: return "PostOnly";
    default:                    return "";
    }
}

inline TimeInForce stringToTimeInForce(const QString& tif)
{
    if (tif == "GTC")       return TimeInForce::GTC;
    if (tif == "IOC")       return TimeInForce::IOC;
    if (tif == "FOK")       return TimeInForce::FOK;
    if (tif == "PostOnly")  return TimeInForce::PostOnly;

    return TimeInForce::Unknown;
}

}

Q_DECLARE_METATYPE(Core::Tools::Interval)
Q_DECLARE_METATYPE(Core::Tools::MarketType)
Q_DECLARE_METATYPE(Core::Tools::OrderType)
Q_DECLARE_METATYPE(Core::Tools::OrderSide)
Q_DECLARE_METATYPE(Core::Tools::OrderStatus)
Q_DECLARE_METATYPE(Core::Tools::ExecType)
