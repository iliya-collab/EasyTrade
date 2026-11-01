#ifndef DONCHIANCHANNEL_HPP
#define DONCHIANCHANNEL_HPP
#include "IIndicator.hpp"
#include <deque>

namespace Core::Trading
{

class DonchianChannel final : public IIndicator
{
public:
    explicit DonchianChannel(int period);

    void update(const Bar& b) override;

    bool ready() const override;
    int  outputs() const override;
    double value(int output = 0) const override;

    bool   prevReady() const;
    double prevUpper() const;
    double prevLower() const;

    void reset() override;

private:
    int m_period;
    std::deque<double> m_hi;
    std::deque<double> m_lo;
    bool m_prevReady = false;
    double m_prevUpper = 0;
    double m_prevLower = 0;
};


}

#endif // DONCHIANCHANNEL_HPP
