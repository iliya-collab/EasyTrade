#ifndef TYPES_HPP
#define TYPES_HPP
#include "StdTypesAdapters.hpp"
#include "StdTypesConvertors.hpp"
#include "Signal.hpp"
#include <variant>

namespace Core::Trading
{

// Состояние сделки. Ведёт его движок или симулятор по фактическим исполнениям, стратегия только читает.
struct BasePosition
{
    double entry = 0;                   // средневзвешенная цена входа (при частичных исполнениях это среднее)
    double qty = 0;                     // размер позиции в базовой монете
    double stop = 0;                    // текущий уровень защитного стопа (цена); 0 = стоп не задан
    double highest = 0;                 // максимальный high с момента входа, по свечам ДО текущей; для трейлинга лонга
    double lowest = 0;                  // минимальный low с момента входа, по свечам ДО текущей; для трейлинга шорта
    int barsHeld = 0;                   // сколько закрытых свечей позиция уже открыта; для выхода по времени
};

struct SpotPosition : BasePosition
{
    double locked = 0;
    double free() const { return qty - locked; }
    bool open() const { return qty > 0; }
};

struct LinearPosition : BasePosition
{
    SideType side = SideType::None;
    double liqPrice = 0;
    double markPrice = 0;
    double leverage = 0;
    double unrealisedPnl = 0;
    bool open() const { return side != SideType::None; }
    double available() const { return qty; }
};

using AnyPosition = std::variant<SpotPosition, LinearPosition>;

inline bool isOpen(const AnyPosition& p)
{
    return std::visit([](const auto& x) { return x.open(); }, p);
}

inline const BasePosition& base(const AnyPosition& p)
{
    return std::visit([](const auto& x) -> const BasePosition& { return x; }, p);
}

inline SideType sideOf(const AnyPosition& p)
{
    return std::visit([](const auto& x) -> SideType {
        using T = std::decay_t<decltype(x)>;
        if constexpr (std::is_same_v<T, LinearPosition>)
            return x.side;
        else
            return x.open() ? SideType::Long : SideType::None;
    }, p);
}

inline double closableQty(const AnyPosition& p)
{
    return std::visit([](const auto& x) {
        using T = std::decay_t<decltype(x)>;
        if constexpr (std::is_same_v<T, SpotPosition>)
            return x.free();
        else
            return x.qty;
    }, p);
}

inline Eh::OrderSide toOrderSide(SignalType t, const AnyPosition& p)
{
    switch (t)
    {
    case SignalType::OpenLong:
        return Eh::OrderSide::Buy;
    case SignalType::OpenShort:
        return Eh::OrderSide::Sell;
    case SignalType::Close:
        return sideOf(p) == SideType::Long ? Eh::OrderSide::Sell : Eh::OrderSide::Buy;
    default:
        return Eh::OrderSide::Unknown;
    }
}

// Всё, что стратегия получает на каждой закрытой свече.
struct Context
{
    InstrumentState inst;   // инструмент для работы
    Bar bar;                // только что закрытая свеча
    AnyPosition pos;        // состояние позиции на момент ДО обработки этой свечи
    WalletState acc;       // состояние счета
};

}

#endif // TYPES_HPP