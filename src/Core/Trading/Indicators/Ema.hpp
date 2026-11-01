#ifndef EMA_HPP
#define EMA_HPP
#include "ClosePriceIndicator.hpp"

namespace Core::Trading
{

class Ema final : public ClosePriceIndicator
{
public:

    explicit Ema(int period);

    bool ready() const override;
    double value(int = 0) const override;
    void reset() override;

private:

    void updateClose(double close) override;

    double m_k = 0;         // k = 2 / (period + 1)
    double m_seedSum = 0;
    double m_value = 0;     // m_value = close * k + m_value * (1 - k)
    int m_period = 0;
    int m_count = 0;

};

}


#endif // EMA_HPP
