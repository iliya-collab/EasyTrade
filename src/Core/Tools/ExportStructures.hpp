#ifndef EXPORTSTRUCTURES_HPP
#define EXPORTSTRUCTURES_HPP
#include "EnumHelpers.hpp"
#include <QString>

namespace Core::Tools
{

struct StdExport
{
    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown;
    QString m_symbol;
};

struct SpecialExportKlines
{;
    EnumHelpers::Interval m_interval = EnumHelpers::Interval::Unknown;
    qint64 m_start = 0;
    qint64 m_end   = 0;
};

}

#endif // EXPORTSTRUCTURES_HPP
