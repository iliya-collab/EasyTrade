#ifndef DONCHIANBREAKOUTENTRY_HPP
#define DONCHIANBREAKOUTENTRY_HPP
#include "Trading/Interfaces/IEntryRule.hpp"
#include "Trading/Indicators/DonchianChannel.hpp"

namespace Core::Trading
{

// Вход: close пробивает максимум (минимум) предыдущих N свечей.
class DonchianBreakout final : public IEntryRule
{
public:

    explicit DonchianBreakout(int period);

    void onBar(const Bar& b) override;
    bool longSignal(const Context& c) override;
    bool shortSignal(const Context& c) override;
    void reset() override;

private:

    DonchianChannel m_dc;
};


}

#endif // DONCHIANBREAKOUTENTRY_HPP
