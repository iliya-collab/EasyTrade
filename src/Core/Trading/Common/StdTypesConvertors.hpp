#ifndef STDTYPESCONVERTORS_HPP
#define STDTYPESCONVERTORS_HPP
#include "StdTypesAdapters.hpp"

namespace Core::Trading
{

inline bool isBuy(Eh::OrderSide s) { return s == Eh::OrderSide::Buy; }

// inline bool isTerminal(Eh::OrderStatus s)
// {
//     return s == Eh::OrderStatus::Filled
//            || s == Eh::OrderStatus::Cancelled
//            || s == Eh::OrderStatus::Rejected;
// }

inline InstrumentState toInstrumentState(const InstrumentInfo& info, const FeeRatesInfo& r)
{
    InstrumentState instState;

    instState.category    = info.m_category;
    instState.baseCoin    = info.m_baseCoin.toStdString();
    instState.quoteCoin   = info.m_quoteCoin.toStdString();
    instState.tickSize    = info.m_tickSize.toDouble();
    instState.minQty      = info.m_minOrderQty.toDouble();
    instState.minNotional = info.m_minNotional.toDouble();

    instState.maxQty    = info.m_maxOrderQty.toDouble();
    instState.maxMktQty = info.m_maxMktQty.toDouble();

    instState.qtyStep = info.m_qtyStep.toDouble();

    if (info.m_category == EnumHelpers::MarketType::Spot)
        instState.maxLeverage = 1.0;
    else
    {
        const double lev = info.m_maxLeverage.toDouble();
        instState.maxLeverage = (lev > 0.0) ? lev : 1.0;
    }

    instState.feeRate = r.m_takerFeeRate.toDouble();
    instState.trading = info.m_status.isEmpty() || (info.m_status == "Trading");

    return instState;
}

inline ExecutionState toExecutionState(const ExecutionInfo& e, const InstrumentInfo& inst)
{
    ExecutionState execState;
    execState.feeBase  = 0.0;
    execState.feeQuote = 0.0;

    execState.execId  = e.m_execId.toStdString();
    execState.orderId = e.m_orderId.toStdString();
    execState.buy     = isBuy(e.m_side);
    execState.price   = e.m_execPrice.toDouble();
    execState.qty     = e.m_execQty.toDouble();
    execState.time    = e.m_execTime;

    const double fee = e.m_execFee.toDouble();
    if (e.m_feeCurrency == inst.m_baseCoin)
        execState.feeBase  = fee;
    else if (e.m_feeCurrency == inst.m_quoteCoin)
        execState.feeQuote = fee;
    return execState;
}

inline OrderState toOrderState(const OrderInfo& o)
{
    OrderState orderState;
    orderState.orderId     = o.m_orderId.toStdString();
    orderState.buy         = isBuy(o.m_side);
    orderState.status      = o.m_status;
    orderState.qty         = o.m_qty.toDouble();
    orderState.cumQty      = o.m_cumExecQty.toDouble();
    orderState.cumValue    = o.m_cumExecValue.toDouble();
    //orderState.positionIdx = o.m_positionIdx;
    orderState.time        = o.m_updatedTime;
    return orderState;
}

// Только фьючерсы. Buy = Long, Sell = Short
inline PositionState toPositionState(const PositionInfo& p)
{
    PositionState posState;
    posState.size = p.m_size.toDouble();
    posState.side = (!(posState.size > 0) || p.m_side == Eh::OrderSide::Unknown)
                        ? SideType::None
                        : isBuy(p.m_side) ? SideType::Long : SideType::Short;
    posState.avgPrice    = p.m_avgPrice.toDouble();
    posState.liqPrice    = p.m_liqPrice.toDouble();
    posState.markPrice   = p.m_markPrice.toDouble();
    posState.leverage    = p.m_leverage.toDouble();
    posState.positionIdx = p.m_positionIdx;
    posState.unrealisedPnl = p.m_unrealisedPnl.toDouble();
    return posState;
}

inline WalletState toWalletState(const WalletInfo& w, const QString& baseCoin)
{
    WalletState s;
    s.time = w.creationTime;

    for (const auto& asset : w.assets)
    {
        if (asset.coin != baseCoin)
            continue;

        const double total = asset.walletBalance.toDouble() - asset.spotBorrow.toDouble();
        s.locked = std::min(asset.locked.toDouble(), std::max(0.0, total));
        s.free = std::max(0.0, total - s.locked);
        break;
    }

    return s;
}

}

#endif // STDTYPESCONVERTORS_HPP
