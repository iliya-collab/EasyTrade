#include "FixedRiskSizer.hpp"

Core::Trading::FixedRiskSizer::FixedRiskSizer(double riskPct) : m_riskPct(riskPct) {}

double Core::Trading::FixedRiskSizer::qty(const Context &c, double stopDistance)
{
    if (!(stopDistance > 0) || !(c.bar.close > 0) || !(c.acc.free > 0))
        return 0.0;

    const double byRisk = c.acc.free * m_riskPct / 100.0 / stopDistance;
    const double byCash = c.acc.free / c.bar.close;

    return std::min(byRisk, byCash);
}
