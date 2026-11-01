#include "DonchianChannel.hpp"
#include "Helpers.hpp"
#include <algorithm>

Core::Trading::DonchianChannel::DonchianChannel(int period) : m_period(checkPeriod(period)) {}

void Core::Trading::DonchianChannel::update(const Bar &b)
{
    if (int(m_hi.size()) == m_period)
    {
        m_prevUpper = *std::max_element(m_hi.begin(), m_hi.end());
        m_prevLower = *std::min_element(m_lo.begin(), m_lo.end());
        m_prevReady = true;
    }
    m_hi.push_back(b.high);
    m_lo.push_back(b.low);
    if (int(m_hi.size()) > m_period)
    {
        m_hi.pop_front();
        m_lo.pop_front();
    }
}

bool Core::Trading::DonchianChannel::ready() const { return int(m_hi.size()) == m_period; }

int Core::Trading::DonchianChannel::outputs() const { return 3; }

double Core::Trading::DonchianChannel::value(int output) const
{
    if (!ready())
        return kNaN;
    const double up = *std::max_element(m_hi.begin(), m_hi.end());
    const double lo = *std::min_element(m_lo.begin(), m_lo.end());
    return output == 0 ? up : output == 1 ? lo : (up + lo) / 2.0;
}

bool Core::Trading::DonchianChannel::prevReady() const { return m_prevReady; }

double Core::Trading::DonchianChannel::prevUpper() const { return m_prevReady ? m_prevUpper : kNaN; }

double Core::Trading::DonchianChannel::prevLower() const { return m_prevReady ? m_prevLower : kNaN; }

void Core::Trading::DonchianChannel::reset()
{
    m_hi.clear();
    m_lo.clear();
    m_prevReady = false;
}
