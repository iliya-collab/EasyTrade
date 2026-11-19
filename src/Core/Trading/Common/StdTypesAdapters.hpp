#ifndef STDTYPESADAPTERS_HPP
#define STDTYPESADAPTERS_HPP
#include <string>
#include "Tools/StdTypes.hpp"

namespace Core::Trading
{

struct InstrumentState
{
    EnumHelpers::MarketType category;
    std::string baseCoin{};
    std::string quoteCoin{};
    double qtyStep = 0;         // шаг количества; 0 = не округлять
    double minQty = 0;          // минимальный размер ордера
    double maxQty = 0;          // максимальный размер ордера; 0 = без ограничения
    double maxMktQty = 0;
    double minNotional = 0;     // минимальная сумма ордера в котируемой валюте
    double tickSize = 0;        // шаг цены (для стопов)
    double maxLeverage = 1.0;   // 1.0 = спот: нельзя купить на сумму больше капитала
    double feeRate = 0;
    bool trading = true;
};

struct WalletState
{
    double free = 0;
    double locked = 0;
    int64_t time{};
};


// Одна свеча в числовом виде. Конвертируется из Kline один раз на входе в стратегию.
struct Bar
{
    int64_t start = 0;    // время открытия свечи
    double open = 0;     // цена первой сделки свечи
    double high = 0;     // максимальная цена за свечу
    double low = 0;      // минимальная цена за свечу
    double close = 0;    // цена последней сделки; у закрытой свечи не меняется
    double volume = 0;   // объём в базовой монете (для BTCUSDT это BTC)
};

// Направление позиции.
enum class SideType
{
    None,    // позиции нет
    Long,    // ставка на рост: купили, прибыль при росте цены
    Short    // ставка на падение: продали, прибыль при падении цены (только фьючерсы)
};

// исполнение
struct ExecutionState
{
    std::string execId;
    std::string orderId;         // к какому ордеру относится
    bool buy{};                    // сторона
    double price{};                // цена этой сделки
    double qty{};                  // количество в базовой монете
    double feeBase{};              // комиссия, списанная в базовой монете
    double feeQuote{};             // комиссия в котируемой монете (отрицательная = рибейт)
    int64_t time{};                // мс
};

// состояние ордера
struct OrderState
{
    std::string orderId;
    bool buy;
    EnumHelpers::OrderStatus status = EnumHelpers::OrderStatus::Unknown;    // Active | Filled | Cancelled | Rejected
    double qty{};
    double cumQty{};                      // исполнено всего
    double cumValue{};                    // стоимость исполненной части
    //int positionIdx;                    // 0 = one-way; не ноль = hedge (игнорируется)
    int64_t time{};
};

// позиция с биржи, только фьючерсы
struct PositionState
{
    SideType side;               // None | Long | Short
    double size{};
    double avgPrice{};
    double liqPrice{};
    double markPrice{};
    double leverage{};
    double unrealisedPnl{};
    int positionIdx{};
    int64_t time{};
};

}

#endif // STDTYPESADAPTERS_HPP
