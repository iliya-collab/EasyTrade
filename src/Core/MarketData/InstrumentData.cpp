#include "InstrumentData.hpp"

Core::InstrumentData::InstrumentData(const InstrumentInfo &info, QObject *parent)
    : QObject(parent), m_info{info} {}

void Core::InstrumentData::updateKline(const KlineInfo &kline)
{
    m_klineStore.updateKline(kline);
    emit klineStoreChanged();
}

void Core::InstrumentData::addHistoricalKlines(const Klines& klines)
{
    if (klines.isEmpty())
        return;
    m_klineStore.addHistoricalKlines(klines);
    emit klineStoreChanged();
}

void Core::InstrumentData::updateTicker(const TickerInfo& ticker)
{
    m_ticker = ticker;
    emit tickerChanged();
}

void Core::InstrumentData::snapshotOrderbook(const OrderbookInfo& orderbook)
{
    m_asks.snapshot(orderbook.m_asks);
    m_bids.snapshot(orderbook.m_bids);
    emit asksChanged();
    emit bidsChanged();
}

void Core::InstrumentData::deltaUpdateOrderbook(const OrderbookInfo& orderbook)
{
    m_asks.deltaUpdate(orderbook.m_asks);
    m_bids.deltaUpdate(orderbook.m_bids);
    emit asksChanged();
    emit bidsChanged();
}

void Core::InstrumentData::updateTrades(const PublicTrades& trades)
{
    if (trades.isEmpty())
        return;
    m_trades.addTradeBatch(trades);
    emit tradesChanged();
}