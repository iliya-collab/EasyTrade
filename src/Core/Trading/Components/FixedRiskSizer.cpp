#include "FixedRiskSizer.hpp"

Core::Trading::FixedRiskSizer::FixedRiskSizer(double riskPct) : m_riskPct(riskPct) {}

double Core::Trading::FixedRiskSizer::qty(const Context &c, double stopDistance)
{
    if (!(stopDistance > 0) || !(c.bar.close > 0) || !(c.equity > 0)) return 0.0;
    const double byRisk = c.equity * m_riskPct / 100.0 / stopDistance;
    const double byCash = c.equity / c.bar.close;
    return std::min(byRisk, byCash);
}
