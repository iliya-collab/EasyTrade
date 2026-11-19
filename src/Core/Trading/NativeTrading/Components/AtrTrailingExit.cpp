#include "AtrTrailingExit.hpp"
#include "Trading/Indicators/Helpers.hpp"
#include <format>

Core::Trading::AtrTrailingExit::AtrTrailingExit(int period, double mult) : m_atr(period), m_mult(mult) {}

void Core::Trading::AtrTrailingExit::onBar(const Bar &b) { m_atr.update(b); }

double Core::Trading::AtrTrailingExit::initialStopDistance(const Context &) const
{
    return m_atr.ready() ? m_atr.value() * m_mult : kNaN;
}

bool Core::Trading::AtrTrailingExit::shouldExit(const Context &c, std::string &why)
{
    if (!isOpen(c.pos) || !m_atr.ready())
        return false;

    const BasePosition& p = base(c.pos);
    const SideType side = sideOf(c.pos);
    const double offset = m_atr.value() * m_mult;

    switch (side)
    {
    case SideType::Long:
    {
        const double hi = p.highest > 0 ? p.highest : p.entry;
        const double trail = hi - offset;
        const double level = std::max(trail, p.stop);       // стоп только поднимается
        if (c.bar.low <= level)
        {
            why = std::format("long stop/trail {:.2f} hit (low {:.2f})", level, c.bar.low);
            return true;
        }
        return false;
    }

    case SideType::Short:
    {
        const double lo = p.lowest > 0 ? p.lowest : p.entry;
        const double trail = lo + offset;
        const double level = p.stop > 0 ? std::min(trail, p.stop) : trail;   // стоп только опускается
        if (c.bar.high >= level)
        {
            why = std::format("short stop/trail {:.2f} hit (high {:.2f})", level, c.bar.high);
            return true;
        }
        return false;
    }

    case SideType::None:
        break;
    }
    return false;
}

void Core::Trading::AtrTrailingExit::reset() { m_atr.reset(); }
