#ifndef RSI_HPP
#define RSI_HPP
#include "ClosePriceIndicator.hpp"

namespace Core::Trading
{

class Rsi final : public ClosePriceIndicator {
public:
    explicit Rsi(int period);

    bool ready() const override;
    double value(int = 0) const override;
    void reset() override;

protected:
    void updateClose(double c) override;

private:
    int m_period;
    bool m_hasPrev = false;
    double m_prev = 0;
    int m_count = 0;
    double m_sumGain = 0;
    double m_sumLoss = 0;
    double m_avgGain = 0;
    double m_avgLoss = 0;
};

}

#endif // RSI_HPP
