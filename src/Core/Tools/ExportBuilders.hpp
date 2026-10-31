#ifndef EXPORTBUILDERS_HPP
#define EXPORTBUILDERS_HPP
#include "ExportStructures.hpp"
#include <QVariantMap>
#include <expected>

namespace Core::Tools
{

inline std::expected<StdExport, QString> buildStdExport(const QVariantMap& params)
{
    StdExport req;

    req.m_category = EnumHelpers::apiStringToMarketType(
        params.value("category").toString());
    req.m_symbol = params.value("symbol").toString();

    if (req.m_category == EnumHelpers::MarketType::Unknown)
        return std::unexpected<QString>("Build std export: \'category\' is required");
    if (req.m_symbol.isEmpty())
        return std::unexpected<QString>("Build std export: \'symbol\' is required");

    return req;
}

inline std::expected<SpecialExportKlines, QString> buildSpecialExportKlines(const QVariantMap& params)
{
    SpecialExportKlines req;

    req.m_start = params.value("start").toLongLong();
    req.m_end   = params.value("end").toLongLong();
    req.m_interval = EnumHelpers::stringToInterval(
        params.value("interval").toString());

    if (req.m_interval == EnumHelpers::Interval::Unknown)
        return std::unexpected<QString>("Build special export for klines: \'interval\' is invalid");
    if (req.m_start <= 0 || req.m_end <= 0)
        return std::unexpected<QString>("Build special export for klines: period \'start\'/\'end\' is required");
    if (req.m_start >= req.m_end)
        return std::unexpected<QString>("Build special export for klines: \'start\' must be before \'end\'");

    return req;
}

}

#endif // EXPORTBUILDERS_HPP
