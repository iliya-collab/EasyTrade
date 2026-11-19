#ifndef ATRTRAILINGEXIT_HPP
#define ATRTRAILINGEXIT_HPP
#include "Trading/NativeTrading/Interfaces/IExitRule.hpp"
#include "Trading/Indicators/Atr.hpp"

namespace Core::Trading
{

class AtrTrailingExit final : public IExitRule
{
public:

    explicit AtrTrailingExit(int period, double mult);

    void onBar(const Bar& b) override;
    double initialStopDistance(const Context&) const override;
    bool shouldExit(const Context& c, std::string& why) override;
    void reset() override;

private:

    Atr m_atr;
    double m_mult;
};


}

#endif // ATRTRAILINGEXIT_HPP
