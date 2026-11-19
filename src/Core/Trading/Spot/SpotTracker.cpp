#include "SpotTracker.hpp"
#include "Trading/Common/MathTrading.hpp"

Core::Trading::SpotTracker::SpotTracker(double minQty) : m_minQty(minQty) {}

void Core::Trading::SpotTracker::onOrder(const OrderState &o) {}

void Core::Trading::SpotTracker::onExec(const ExecutionState &e)
{
    if (!m_seenExec.emplace(e.execId, e.time).second)
        return;

    applyBasis(e);
    refreshEntry();
}

void Core::Trading::SpotTracker::onBarClosed(const Bar &b)
{
    if (m_pos.open())
        Math::advanceBar(m_pos, b);
}

void Core::Trading::SpotTracker::setStop(double stop) { m_pos.stop = stop; }

void Core::Trading::SpotTracker::reset()
{
    resetPosition();
    m_seenExec.clear();
    m_walletTime = 0;
}

void Core::Trading::SpotTracker::onWallet(const WalletState &w)
{
    if (w.time < m_walletTime)
        return;

    m_walletTime = w.time;

    const double total = w.free + w.locked;
    const bool wasOpen = m_pos.open();

    if (isDust(total))
    {
        resetPosition();
        return;
    }

    m_pos.qty = total;
    m_pos.locked = std::min(w.locked, total);

    if (!wasOpen)
    {
        m_pos.barsHeld = 0;
        m_pos.lowest = m_pos.highest = m_pos.entry;
    }
}

bool Core::Trading::SpotTracker::entryKnown() const
{
    return !m_pos.open() || m_pos.entry > 0;
}

void Core::Trading::SpotTracker::applyBasis(const ExecutionState &e)
{
    if (e.buy)
    {
        m_heldQty   += e.qty - e.feeBase;
        m_costQuote += e.price * e.qty + e.feeQuote;
    }
    else
    {
        const double avg = (m_heldQty > Math::kEps) ? m_costQuote / m_heldQty : 0.0;
        m_heldQty   = std::max(0.0, m_heldQty - (e.qty + e.feeBase));
        m_costQuote = avg * m_heldQty;
        if (isDust(m_heldQty))
            m_heldQty = m_costQuote = 0;
    }

}

void Core::Trading::SpotTracker::refreshEntry()
{
    m_pos.entry = (m_heldQty > Math::kEps) ? m_costQuote / m_heldQty : 0.0;

    if (m_pos.open() && m_pos.highest <= 0)
        m_pos.lowest = m_pos.highest = m_pos.entry;
}

bool Core::Trading::SpotTracker::isDust(double qty) const
{
    return qty <= Math::kEps || (m_minQty > 0 && qty < m_minQty);
}

void Core::Trading::SpotTracker::resetPosition()
{
    m_pos = SpotPosition{};
    m_heldQty = 0;
    m_costQuote = 0;
}

Core::Trading::AnyPosition Core::Trading::SpotTracker::snapshot() const
{
    return m_pos;
}
