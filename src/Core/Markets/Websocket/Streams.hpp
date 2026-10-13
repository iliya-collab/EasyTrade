#pragma once
#include "Tools/EnumHelpers.hpp"

namespace Core::Markets
{

// Публичные потоки
enum class PublicStreams
{
    // Тикер
    Ticker,

    // Стакан - по одному значению на глубину
    Orderbook1,
    Orderbook50,
    Orderbook200,
    Orderbook1000,

    // Стаканы только для опционов
    Orderbook25,
    Orderbook100,

    // Свечи - по одному значению на интервал
    Kline1m,
    Kline3m,
    Kline5m,
    Kline15m,
    Kline30m,
    Kline1h,
    Kline2h,
    Kline4h,
    Kline6h,
    Kline12h,
    Kline1D,
    Kline1W,
    Kline1M,

    // Публичные сделки
    PublicTrade
};

inline Core::Tools::EnumHelpers::Interval klineInterval(PublicStreams s)
{
    using Core::Tools::EnumHelpers;
    switch (s)
    {
    case PublicStreams::Kline1m:  return EnumHelpers::Interval::Min1;
    case PublicStreams::Kline3m:  return EnumHelpers::Interval::Min3;
    case PublicStreams::Kline5m:  return EnumHelpers::Interval::Min5;
    case PublicStreams::Kline15m: return EnumHelpers::Interval::Min15;
    case PublicStreams::Kline30m: return EnumHelpers::Interval::Min30;
    case PublicStreams::Kline1h:  return EnumHelpers::Interval::Hour1;
    case PublicStreams::Kline2h:  return EnumHelpers::Interval::Hour2;
    case PublicStreams::Kline4h:  return EnumHelpers::Interval::Hour4;
    case PublicStreams::Kline6h:  return EnumHelpers::Interval::Hour6;
    case PublicStreams::Kline12h: return EnumHelpers::Interval::Hour12;
    case PublicStreams::Kline1D:  return EnumHelpers::Interval::Day;
    case PublicStreams::Kline1W:  return EnumHelpers::Interval::Week;
    case PublicStreams::Kline1M:  return EnumHelpers::Interval::Month;
    default:                      return EnumHelpers::Interval::Unknown;
    }
}

inline bool isKlineStream(PublicStreams s) { return klineInterval(s) != Core::Tools::EnumHelpers::Interval::Unknown; }

inline int orderbookDepth(PublicStreams s)
{
    switch (s)
    {
    case PublicStreams::Orderbook1:    return 1;
    case PublicStreams::Orderbook25:   return 25;
    case PublicStreams::Orderbook50:   return 50;
    case PublicStreams::Orderbook100:  return 100;
    case PublicStreams::Orderbook200:  return 200;
    case PublicStreams::Orderbook1000: return 1000;
    default:                           return 0;
    }
}
inline bool isOrderbookStream(PublicStreams s) { return orderbookDepth(s) > 0; }

// Приватные потокм
enum class PrivateStreams
{
    Wallet,             // Баланс кошелек
    Order,              // Отслеживание ордеров
    Position,           // Контроль позиции
    Execution           // История сделок
};

}