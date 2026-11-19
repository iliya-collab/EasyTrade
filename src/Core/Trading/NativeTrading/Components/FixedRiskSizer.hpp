#ifndef FIXEDRISKSIZER_HPP
#define FIXEDRISKSIZER_HPP
#include "Trading/NativeTrading/Interfaces/ISizer.hpp"

namespace Core::Trading
{

class FixedRiskSizer final : public ISizer
{
public:

    explicit FixedRiskSizer(double riskPct);
    double qty(const Context& c, double stopDistance) override;

private:

    double m_riskPct;
};


}

#endif // FIXEDRISKSIZER_HPP
