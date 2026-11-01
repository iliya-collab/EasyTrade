#ifndef EMATRENDFILTER_HPP
#define EMATRENDFILTER_HPP
#include "Trading/Interfaces/IFilter.hpp"
#include "Trading/Indicators/Ema.hpp"

namespace Core::Trading
{

// Фильтр: торгуем в лонг только выше EMA
class EmaTrendFilter final : public IFilter
{
public:

    explicit EmaTrendFilter(int period);
    void onBar(const Bar& b) override;
    bool allows(const Context& c, SideType side, std::string& whyNot) override;
    void reset() override;

private:

    int m_period;
    Ema m_ema;

};

}

#endif // EMATRENDFILTER_HPP
