#ifndef ORDERBOOKINFO_HPP
#define ORDERBOOKINFO_HPP
#include "EnumHelpers.hpp"

namespace Core::Tools::Types
{

// ==========================================================================================
//  Стакан заявок
// ==========================================================================================

using OrderbookLevelInfo = QMap<QString, QString>; // уровнь (цена, объем)

struct OrderbookInfo
{
    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown;
    QString m_symbol = "";          // Название пары
    OrderbookLevelInfo m_bids{};    // Покупки
    OrderbookLevelInfo m_asks{};    // Продажи
};

}

#endif // ORDERBOOKINFO_HPP
