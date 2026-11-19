#ifndef KLINEINFO_HPP
#define KLINEINFO_HPP
#include "EnumHelpers.hpp"

namespace Core::Tools::Types
{

// ==========================================================================================
//  Свеча
// ==========================================================================================

struct KlineInfo
{
    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown;
    QString m_symbol = "";
    EnumHelpers::Interval m_interval = EnumHelpers::Interval::Unknown;
    QString m_open {};
    QString m_close {};
    QString m_high {};
    QString m_low {};
    qint64 m_start {};
    qint64 m_end {};
    QString m_volume {};
    QString m_turnover {};
    bool m_confirm = false;
};

using Klines = QList<KlineInfo>;

}

#endif // KLINEINFO_HPP
