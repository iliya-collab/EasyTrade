#include "KlineStore.hpp"

namespace Core::Tools {

const Klines KlineStore::s_empty{};

Klines KlineStore::series(const QString& timeframe) const
{
    auto it = m_klineSeries.constFind(timeframe);
    if (it == m_klineSeries.constEnd())
        return s_empty;
    return it->values();
}

int KlineStore::seriesSize(const QString &timeframe) const
{
    auto it = m_klineSeries.constFind(timeframe);
    return it == m_klineSeries.constEnd() ? 0 : it->size();
}

bool KlineStore::lastKline(const QString& timeframe, KlineInfo& out) const
{
    auto it = m_klineSeries.constFind(timeframe);
    if (it == m_klineSeries.constEnd() || it->isEmpty())
        return false;
    out = std::prev(it->constEnd()).value();
    return true;
}

bool KlineStore::hasTimeframe(const QString& timeframe) const
{
    auto it = m_klineSeries.constFind(timeframe);
    return it != m_klineSeries.constEnd();
}

void KlineStore::updateKline(const KlineInfo& kline)
{
    QString tf = EnumHelpers::intervalToString(kline.m_interval);
    auto& bucket = m_klineSeries[tf];

    auto it = bucket.find(kline.m_start);
    if (it == bucket.end())
    {
        addKline(kline);
        return;
    }

    it.value().m_open       = kline.m_open;
    it.value().m_close      = kline.m_close;
    it.value().m_volume     = kline.m_volume;
    it.value().m_turnover   = kline.m_turnover;
    it.value().m_confirm    = kline.m_confirm;

    emit klineUpdated(tf);
}

void KlineStore::addKline(const KlineInfo& kline)
{
    QString tf = EnumHelpers::intervalToString(kline.m_interval);
    auto& bucket = m_klineSeries[tf];

    if (bucket.contains(kline.m_start))
    {
        updateKline(kline);
        return;
    }

    bucket.insert(kline.m_start, kline);
    emit klineAppended(tf);
}

void KlineStore::addHistoricalKlines(const Klines& klines)
{
    if (klines.isEmpty())
        return;

    QString tf = EnumHelpers::intervalToString(klines.first().m_interval);
    auto& bucket = m_klineSeries[tf];

    for (const auto& kline : klines)
        bucket.insert(kline.m_start, kline);

    emit seriesReset(tf);
}

void KlineStore::clear()
{
    m_klineSeries.clear();
    emit allCleared();
}

void KlineStore::clearTimeframe(const QString& timeframe)
{
    m_klineSeries.remove(timeframe);
    emit seriesReset(timeframe);
}

}