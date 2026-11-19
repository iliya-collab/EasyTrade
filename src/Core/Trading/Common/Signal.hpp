#ifndef SIGNAL_HPP
#define SIGNAL_HPP
#include <string>

namespace Core::Trading
{

// Что стратегия просит сделать.
enum class SignalType
{
    None,       // ничего не делать (в reason может быть причина пропуска, например "Skip: ...")
    OpenLong,   // открыть лонг
    OpenShort,  // открыть шорт
    Close       // закрыть текущую позицию целиком
};

struct Signal
{
    SignalType type = SignalType::None;  // действие
    double qty = 0;       // сколько открыть (Buy/Sell) или закрыть (Close), в базовой монете
    double stop = 0;      // цена защитного стопа для нового входа; для Close и None не используется (0)
    std::string reason;   // текст для журнала сигналов: почему вошли, вышли или пропустили
};

}

#endif // SIGNAL_HPP
