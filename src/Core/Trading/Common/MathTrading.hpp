#ifndef MATHTRADING_HPP
#define MATHTRADING_HPP
#include <cmath>
#include <expected>
#include <string>
#include "Trading/Common/Types.hpp"

namespace Core::Trading::Math
{

constexpr double kEps = 1e-12;

inline double roundDownToStep(double v, double step)
{
    if (step <= 0)
        return v;
    return std::floor(v / step + kEps) * step;
}

inline double roundToTick(double price, double tick)
{
    if (tick <= 0)
        return price;
    return std::round(price / tick) * tick;
}

inline std::expected<double, std::string> normalizeQty(double qty, double price, const InstrumentState& s, bool market)
{
    if (!s.trading)
        return std::unexpected("Instrument is not trading");

    const double cap = (market && s.maxMktQty > 0) ? s.maxMktQty : s.maxQty;
    if (cap > 0)
        qty = std::min(qty, cap);
    qty = roundDownToStep(qty, s.qtyStep);

    if (qty <= 0 || qty < s.minQty)
        return std::unexpected("Qty below min qty");

    if (s.minNotional > 0 && qty * price < s.minNotional)
        return std::unexpected("Notional below min notional");

    return qty;
}

inline void advanceBar(BasePosition& pos, const Bar& b)
{
    pos.highest = std::max(pos.highest, b.high);
    pos.lowest = (pos.lowest > 0) ? std::min(pos.lowest, b.low) : b.low;
    ++pos.barsHeld;
}

}

#endif // MATHTRADING_HPP
