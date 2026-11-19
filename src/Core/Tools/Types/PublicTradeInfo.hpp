#ifndef PUBLICTRADEINFO_HPP
#define PUBLICTRADEINFO_HPP
#include "EnumHelpers.hpp"

namespace Core::Tools::Types
{
// ==========================================================================================
//  Публичная лента сделок
// ==========================================================================================

struct PublicTradeInfo
{
    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown;
    QString m_symbol = "";
    EnumHelpers::OrderSide m_side = EnumHelpers::OrderSide::Unknown;
    QString m_price {};
    QString m_volume {};
    QString m_turnover {};
    qint64 m_tradeTime = 0;
};

using PublicTrades = QList<PublicTradeInfo>;

}

#endif // PUBLICTRADEINFO_HPP
