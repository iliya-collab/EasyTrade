#ifndef SMA_HPP
#define SMA_HPP
#include <deque>
#include "ClosePriceIndicator.hpp"

namespace Core::Trading
{

class Sma final : public ClosePriceIndicator
{
public:
    explicit Sma(int period);

    bool   ready() const override;
    double value(int = 0) const override;
    void   reset() override;

protected:
    void updateClose(double c) override;

private:
    int m_period;
    std::deque<double> m_win;
    double m_sum = 0;
};


}

#endif // SMA_HPP
