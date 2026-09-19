#include "Converters.hpp"

// --- PublicTradeItem ---

QList<QPair<QString, QVariant> > Core::Tools::Exporter::Converters::toField(const PublicTradeItem &t)
{
    return {
        { "category", marketTypeToString(t.m_category) },
        { "symbol",   t.m_symbol },
        { "side",     orderSideToString(t.m_side) },
        { "price",    t.m_price },
        { "volume",   t.m_volume },
        { "turnover", t.m_turnover },
        { "time",     static_cast<qint64>(t.m_tradeTime) }
    };
}

// --- Kline ---

QList<QPair<QString, QVariant> > Core::Tools::Exporter::Converters::toField(const Kline &k)
{
    return {
        { "category", marketTypeToString(k.m_category) },
        { "symbol",   k.m_symbol },
        { "interval", intervalToString(k.m_interval) },
        { "open",     k.m_open },
        { "close",    k.m_close },
        { "high",     k.m_high },
        { "low",      k.m_low },
        { "start",    static_cast<qint64>(k.m_start) },
        { "end",      static_cast<qint64>(k.m_end) },
        { "volume",   k.m_volume },
        { "turnover", k.m_turnover },
        { "confirm",  k.m_confirm }
    };
}

// --- OrderInfo ---

QList<QPair<QString, QVariant> > Core::Tools::Exporter::Converters::toField(const OrderInfo &o)
{
    return {
        { "category",     marketTypeToString(o.m_category) },
        { "orderId",      o.m_orderId },
        { "orderLinkId",  o.m_orderLinkId },
        { "symbol",       o.m_symbol },
        { "side",         orderSideToString(o.m_side) },
        { "orderType",    orderTypeToString(o.m_orderType) },
        { "price",        o.m_price },
        { "qty",          o.m_qty },
        { "status",       orderStatusToString(o.m_status) },
        { "positionIdx",  o.m_positionIdx },
        { "leavesQty",    o.m_leavesQty },
        { "cumExecQty",   o.m_cumExecQty },
        { "cumExecValue", o.m_cumExecValue },
        { "cumExecFee",   o.m_cumExecFee },
        { "rejectReason", o.m_rejectReason },
        { "createdTime",  static_cast<qint64>(o.m_createdTime) },
        { "updatedTime",  static_cast<qint64>(o.m_updatedTime) }
    };
}

// --- ExecutionInfo ---

QList<QPair<QString, QVariant> > Core::Tools::Exporter::Converters::toField(const ExecutionInfo &e)
{
    return {
        { "category",    marketTypeToString(e.m_category) },
        { "symbol",      e.m_symbol },
        { "execId",      e.m_execId },
        { "orderId",     e.m_orderId },
        { "orderLinkId", e.m_orderLinkId },
        { "side",        orderSideToString(e.m_side) },
        { "execPrice",   e.m_execPrice },
        { "execQty",     e.m_execQty },
        { "execValue",   e.m_execValue },
        { "execType",    execTypeToString(e.m_execType) },
        { "execFee",     e.m_execFee },
        { "feeCurrency", e.m_feeCurrency },
        { "isMaker",     e.m_isMaker },
        { "execTime",    static_cast<qint64>(e.m_execTime) }
    };
}

// --- PositionInfo ---

QList<QPair<QString, QVariant> > Core::Tools::Exporter::Converters::toField(const PositionInfo &p)
{
    return {
        { "category",         marketTypeToString(p.m_category) },
        { "symbol",           p.m_symbol },
        { "side",             orderSideToString(p.m_side) },
        { "size",             p.m_size },
        { "positionIdx",      p.m_positionIdx },
        { "avgPrice",         p.m_avgPrice },
        { "liqPrice",         p.m_liqPrice },
        { "markPrice",        p.m_markPrice },
        { "leverage",         p.m_leverage },
        { "positionIM",       p.m_positionIM },
        { "positionMM",       p.m_positionMM },
        { "unrealisedPnl",    p.m_unrealisedPnl },
        { "cumRealisedPnl",   p.m_cumRealisedPnl },
        { "adlRankIndicator", p.m_adlRankIndicator }
    };
}