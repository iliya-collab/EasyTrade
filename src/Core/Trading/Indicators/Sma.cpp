#include "Sma.hpp"
#include "Helpers.hpp"

Core::Trading::Sma::Sma(int period)
    : m_period(checkPeriod(period)) {}

bool Core::Trading::Sma::ready() const { return int(m_win.size()) == m_period; }

double Core::Trading::Sma::value(int) const { return ready() ? m_sum / m_period : kNaN; }

void Core::Trading::Sma::reset() { m_win.clear(); m_sum = 0; }

void Core::Trading::Sma::updateClose(double c)
{
    m_win.push_back(c);
    m_sum += c;
    if (int(m_win.size()) > m_period)
    {
        m_sum -= m_win.front();
        m_win.pop_front();
    }
}
