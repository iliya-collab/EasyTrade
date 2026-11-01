#include "EmaTrendFilter.hpp"
#include <format>

Core::Trading::EmaTrendFilter::EmaTrendFilter(int period) : m_period(period), m_ema(period) {}

void Core::Trading::EmaTrendFilter::onBar(const Bar &b) { m_ema.update(b); }

bool Core::Trading::EmaTrendFilter::allows(const Context &c, SideType side, std::string &whyNot)
{
    if (!m_ema.ready())
    {
        whyNot = "EMA not ready";
        return false;
    }

    const double close = c.bar.close;
    const double ema   = m_ema.value();

    switch (side)
    {
    case SideType::Long:
        if (close <= ema)
        {
            whyNot = std::format("long: close {:.2f} <= EMA{} {:.2f}", close, m_period, ema);
            return false;
        }
        return true;

    case SideType::Short:
        if (close >= ema)
        {
            whyNot = std::format("short: close {:.2f} >= EMA{} {:.2f}", close, m_period, ema);
            return false;
        }
        return true;

    case SideType::None:
        break;
    }

    whyNot = "no side";
    return false;
}

void Core::Trading::EmaTrendFilter::reset() { m_ema.reset(); }
