#include "LinearTracker.hpp"
#include "Trading/Common/MathTrading.hpp"

Core::Trading::LinearTracker::LinearTracker() = default;

void Core::Trading::LinearTracker::onOrder(const OrderState &o) {}

void Core::Trading::LinearTracker::onExec(const ExecutionState &e) {}

void Core::Trading::LinearTracker::onBarClosed(const Bar &b)
{
    if (m_pos.open())
        Math::advanceBar(m_pos, b);
}

void Core::Trading::LinearTracker::setStop(double stop) { m_pos.stop = stop; }

void Core::Trading::LinearTracker::reset()
{
    m_pos = LinearPosition{};
}

void Core::Trading::LinearTracker::onPos(const PositionState &p)
{
    if (p.positionIdx != 0)
        return;

    if (p.side == SideType::None || p.size <= 0)
    {
        m_pos = LinearPosition{};
        return;
    }

    const bool opened = !m_pos.open() || p.side != m_pos.side;

    m_pos.side          = p.side;
    m_pos.qty           = p.size;
    m_pos.entry         = p.avgPrice;
    m_pos.liqPrice      = p.liqPrice;
    m_pos.markPrice     = p.markPrice;
    m_pos.leverage      = p.leverage;
    m_pos.unrealisedPnl = p.unrealisedPnl;

    if (opened)
    {
        m_pos.highest = m_pos.lowest = p.avgPrice;
        m_pos.barsHeld = 0;
        m_pos.stop = 0;
    }
}

Core::Trading::AnyPosition Core::Trading::LinearTracker::snapshot() const
{
    return m_pos;
}

