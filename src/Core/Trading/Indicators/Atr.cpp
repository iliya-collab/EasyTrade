#include "Atr.hpp"
#include "Helpers.hpp"
#include <cmath>
#include <algorithm>

Core::Trading::Atr::Atr(int period) : m_period(checkPeriod(period)) {}

void Core::Trading::Atr::update(const Bar &b)
{
    double tr = b.high - b.low;
    if (m_hasPrev)
        tr = std::max({tr, std::abs(b.high - m_prevClose), std::abs(b.low - m_prevClose)});
    m_prevClose = b.close;
    m_hasPrev = true;

    if (m_count < m_period)
    {
        m_seedSum += tr;
        if (++m_count == m_period)
            m_value = m_seedSum / m_period;
    }
    else
        m_value = (m_value * (m_period - 1) + tr) / m_period;

}

bool Core::Trading::Atr::ready() const { return m_count >= m_period; }

double Core::Trading::Atr::value(int) const { return ready() ? m_value : kNaN; }

void Core::Trading::Atr::reset()
{
    m_hasPrev = false;
    m_prevClose = 0;
    m_count = 0;
    m_seedSum = 0;
    m_value = 0;
}
