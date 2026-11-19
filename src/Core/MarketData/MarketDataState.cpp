#include "MarketDataState.hpp"
#include "Tools/Logging.hpp"

namespace Core {

    MarketDataState::MarketDataState(QObject *parent) : QObject(parent)
    {
        m_availableInsts = std::make_shared<InstrumentModel>(this);
        m_openedInsts = std::make_shared<InstrumentModel>(this);

        m_empty = new InstrumentData(InstrumentInfo{}, this);
        QQmlEngine::setObjectOwnership(m_empty, QQmlEngine::CppOwnership);
        resetCurrent();
    }

    InstrumentData *MarketDataState::current() const { return m_current; }

    InstrumentData* MarketDataState::find(Eh::MarketType m, const QString& symbol) const
    {
        return m_data.value(InstrumentInfo::makeId(m, symbol), nullptr);
    }

    InstrumentData* MarketDataState::ensure(const InstrumentInfo& info)
    {
        const QString key = info.id();
        if (auto* d = m_data.value(key, nullptr))
            return d;

        auto* d = new InstrumentData(info, this);
        QQmlEngine::setObjectOwnership(d, QQmlEngine::CppOwnership);
        m_data.insert(key, d);
        return d;
    }

    void MarketDataState::remove(const QString& key)
    {
        auto* d = m_data.take(key);

        if (!d)
            return;

        if (m_current == d)
        {
            resetCurrent();
            emit currentChanged();
        }

        d->deleteLater();
    }
    void MarketDataState::setCurrent(const InstrumentInfo &info)
    {
        auto* d = ensure(info);
        if (m_current == d)
            return;
        m_current = d;
        emit currentChanged();
    }

    void MarketDataState::resetCurrent()
    {
        m_current = m_empty;
    }

    InstrumentData* MarketDataState::instrument(const InstrumentInfo& info) const
    {
        return find(info.m_category, info.m_symbol);
    }

    void MarketDataState::updateTicker(const TickerInfo& t)
    {
        if (auto* d = find(t.m_category, t.m_symbol))
            d->updateTicker(t);
    }

    void MarketDataState::snapshotOrderbook(const OrderbookInfo& ob)
    {
        if (auto* d = find(ob.m_category, ob.m_symbol))
            d->snapshotOrderbook(ob);
    }

    void MarketDataState::deltaUpdateOrderbook(const OrderbookInfo& ob)
    {
        if (auto* d = find(ob.m_category, ob.m_symbol))
            d->deltaUpdateOrderbook(ob);
    }

    void MarketDataState::updateKline(const KlineInfo& k)
    {
        if (auto* d = find(k.m_category, k.m_symbol))
            d->updateKline(k);
    }

    void MarketDataState::addHistoricalKlines(const Klines& ks)
    {
        if (ks.isEmpty())
            return;
        const auto& f = ks.first();
        if (auto* d = find(f.m_category, f.m_symbol))
            d->addHistoricalKlines(ks);
    }

    void MarketDataState::updateTrades(const PublicTrades& ts)
    {
        if (ts.isEmpty())
            return;
        const auto& f = ts.first();
        if (auto* d = find(f.m_category, f.m_symbol))
            d->updateTrades(ts);
    }

    void MarketDataState::openInstrument(const InstrumentInfo &info)
    {
        const QString key = info.id();
        if (m_openedKeys.contains(key))
            return;

        m_openedKeys.insert(key);

        ensure(info);
        m_openedInsts->addInstrument(info);
        m_availableInsts->removeInstrument(info);

        emit availableInstsChanged();
        emit openedInstsChanged();
    }

    void MarketDataState::closeInstrument(const InstrumentInfo &info)
    {
        const QString key = info.id();
        if (!m_openedKeys.remove(key))
            return;

        remove(key);
        m_openedInsts->removeInstrument(info);
        m_availableInsts->addInstrument(info);

        emit availableInstsChanged();
        emit openedInstsChanged();
    }

    bool MarketDataState::isOpened(const InstrumentInfo& info) const
    {
        return m_openedKeys.contains(info.id());
    }

    Instruments MarketDataState::collectOpened() const
    {
        Instruments out;
        out.reserve(m_openedKeys.size());

        for (const QString& k : m_openedKeys)
            if (auto* d = m_data.value(k, nullptr))
                out.push_back(d->getInfo());

        return out;
    }

    void MarketDataState::uploadInstruments(const Instruments& instruments)
    {
        m_availableInsts->upsertInstruments(instruments);
        emit availableInstsChanged();
    }

    void MarketDataState::updatePingMs(const QString& id, qint64 pingMs)
    {
        m_pingMs = pingMs;
        emit pingMsChanged();
    }

    qint64 MarketDataState::getPingMs() const { return m_pingMs; }

    InstrumentModel *MarketDataState::getAvailableInsts() const { return m_availableInsts.get(); }

    InstrumentModel *MarketDataState::getOpenedInsts() const
    {
        return m_openedInsts.get();
    }

}

