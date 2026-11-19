#include "DonchianBreakoutEntry.hpp"

Core::Trading::DonchianBreakout::DonchianBreakout(int period)
    : m_dc(period) {}

void Core::Trading::DonchianBreakout::onBar(const Bar &b) { m_dc.update(b); }

bool Core::Trading::DonchianBreakout::longSignal(const Context &c)
{
    return m_dc.prevReady() && c.bar.close > m_dc.prevUpper();
}

bool Core::Trading::DonchianBreakout::shortSignal(const Context &c)
{
    return m_dc.prevReady() && c.bar.close < m_dc.prevLower();
}

void Core::Trading::DonchianBreakout::reset() { m_dc.reset(); }
