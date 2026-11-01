#include "Ema.hpp"
#include "Helpers.hpp"

Core::Trading::Ema::Ema(int period)
    : m_period(checkPeriod(period)), m_k(2.0 / (period + 1)) {}

void Core::Trading::Ema::updateClose(double c)
{
    if (m_count < m_period)
    {
        m_seedSum += c;
        if (++m_count == m_period)
            m_value = m_seedSum / m_period;
    } else
        m_value = m_k * c + (1.0 - m_k) * m_value;
}

bool Core::Trading::Ema::ready() const { return m_count >= m_period; }

double Core::Trading::Ema::value(int) const { return ready() ? m_value : kNaN; }

void Core::Trading::Ema::reset() { m_count = 0; m_seedSum = 0; m_value = 0; }