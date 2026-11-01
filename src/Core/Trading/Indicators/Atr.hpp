#ifndef ATR_HPP
#define ATR_HPP
#include "IIndicator.hpp"

namespace Core::Trading
{

class Atr final : public IIndicator
{
public:
    explicit Atr(int period);

    void update(const Bar& b) override;

    bool ready() const override;
    double value(int = 0) const override;
    void reset() override;

private:
    int m_period;
    bool m_hasPrev = false;
    double m_prevClose = 0;
    int m_count = 0;
    double m_seedSum = 0;
    double m_value = 0;
};


}

#endif // ATR_HPP
