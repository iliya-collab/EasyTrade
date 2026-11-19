#include "CompositeStrategy.hpp"

Core::Trading::CompositeStrategy::CompositeStrategy(
    std::unique_ptr<IEntryRule> entry,
    std::unique_ptr<IExitRule> exit,
    std::vector<std::unique_ptr<IFilter> > filters,
    std::unique_ptr<ISizer> sizer)
    :   m_entry(std::move(entry)),
        m_exit(std::move(exit)),
        m_filters(std::move(filters)),
        m_sizer(std::move(sizer))
{}

void Core::Trading::CompositeStrategy::setInst(const InstrumentState &inst)
{
    m_inst = inst;
}

void Core::Trading::CompositeStrategy::reset()
{
    m_entry->reset();
    for (auto& f : m_filters)
        f->reset();
    m_exit->reset();
}

Core::Trading::Signal Core::Trading::CompositeStrategy::onClosedKline(const Context &ctx)
{
    m_entry->onBar(ctx.bar);
    for (auto& f : m_filters)
        f->onBar(ctx.bar);
    m_exit->onBar(ctx.bar);

    // Есть позиция: только выход
    if (isOpen(ctx.pos))
    {
        std::string why;
        if (m_exit->shouldExit(ctx, why))
            return {SignalType::Close, base(ctx.pos).qty, 0.0, why};
        return {};
    }

    // Нет позиции: вход -> фильтры -> стоп -> размер
    SideType side = SideType::None;
    const bool wantLong  = m_entry->longSignal(ctx);
    const bool wantShort = m_entry->shortSignal(ctx);

    if (wantLong && wantShort)
        return {SignalType::None, 0, 0, "Skip: long and short signals at once"};   // конфликт правил

    if (wantLong)
        side = SideType::Long;
    if (wantShort)
        side = SideType::Short;

    if (side == SideType::None)
        return {};

    for (auto& f : m_filters)
    {
        std::string why;
        if (!f->allows(ctx, why))
            return {SignalType::None, 0.0, 0.0, "Skip: " + why};
    }

    const double dist = m_exit->initialStopDistance(ctx);
    if (!(dist > 0))
        return {SignalType::None, 0.0, 0.0, "Skip: stop distance unavailable"};

    const double qty = m_sizer->qty(ctx, dist);
    if (!(qty > 0))
        return {SignalType::None, 0.0, 0.0, "Skip: qty = 0"};

    switch (side)
    {
    case SideType::Long:
        return {SignalType::OpenLong,  qty, ctx.bar.close - dist, "Long entry"};
    case SideType::Short:
        return {SignalType::OpenShort, qty, ctx.bar.close + dist, "Short entry"};
    case SideType::None:
        break;
    }

    return {};
}
