#include "Rsi.hpp"
#include "Helpers.hpp"

Core::Trading::Rsi::Rsi(int period)
    : m_period(checkPeriod(period)) {}

bool Core::Trading::Rsi::ready() const { return m_count >= m_period; }

double Core::Trading::Rsi::value(int) const
{
    if (!ready())
        return kNaN;
    if (m_avgLoss == 0.0)
        return m_avgGain == 0.0 ? 50.0 : 100.0;
    const double rs = m_avgGain / m_avgLoss;
    return 100.0 - 100.0 / (1.0 + rs);
}

void Core::Trading::Rsi::reset()
{
    m_hasPrev = false; m_prev = 0; m_count = 0;
    m_sumGain = m_sumLoss = m_avgGain = m_avgLoss = 0;
}

void Core::Trading::Rsi::updateClose(double c)
{
    if (!m_hasPrev)
    {
        m_prev = c;
        m_hasPrev = true;
        return;
    }

    const double ch   = c - m_prev;
    const double gain = ch > 0 ? ch : 0.0;
    const double loss = ch < 0 ? -ch : 0.0;
    m_prev = c;

    if (m_count < m_period)
    {
        m_sumGain += gain;
        m_sumLoss += loss;
        if (++m_count == m_period)
        {
            m_avgGain = m_sumGain / m_period;
            m_avgLoss = m_sumLoss / m_period;
        }
    } else
    {
        m_avgGain = (m_avgGain * (m_period - 1) + gain) / m_period;
        m_avgLoss = (m_avgLoss * (m_period - 1) + loss) / m_period;
    }
}
