#include "KlineSeriesModel.hpp"

namespace Core::Tools {

    KlineSeriesModel::KlineSeriesModel(QObject* parent)
        : QAbstractListModel(parent)
    {
    }

    int KlineSeriesModel::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;
        return m_rows.size();
    }

    QVariant KlineSeriesModel::data(const QModelIndex& index, int role) const
    {
        if (!index.isValid())
            return {};

        const int row = index.row();
        if (row < 0 || row >= m_rows.size())
            return {};

        const Kline& k = m_rows.at(row);

        switch (role) {
        case OpenRole:      return k.m_open.toDouble();
        case CloseRole:     return k.m_close.toDouble();
        case HighRole:      return k.m_high.toDouble();
        case LowRole:       return k.m_low.toDouble();
        case VolumeRole:    return k.m_volume.toDouble();
        case TurnoverRole:  return k.m_turnover.toDouble();
        case TimeRole:      return k.m_start;
        case ConfirmRole:   return k.m_confirm;
        default:            return {};
        }
    }

    QHash<int, QByteArray> KlineSeriesModel::roleNames() const
    {
        return {
            {OpenRole,      "open"},
            {CloseRole,     "close"},
            {HighRole,      "high"},
            {LowRole,       "low"},
            {VolumeRole,    "volume"},
            {TurnoverRole,  "turnover"},
            {TimeRole,      "time"},
            {ConfirmRole,   "isConfirm"}
        };
    }

    void KlineSeriesModel::setKlineStore(KlineStore* source)
    {
        if (m_source == source)
            return;

        if (m_source)
            disconnect(m_source, nullptr, this, nullptr);

        m_source = source;

        if (m_source)
        {
            connect(m_source, &KlineStore::seriesReset, this, &KlineSeriesModel::onSeriesReset, Qt::UniqueConnection);
            connect(m_source, &KlineStore::klineAppended, this, &KlineSeriesModel::onKlineAppended, Qt::UniqueConnection);
            connect(m_source, &KlineStore::klineUpdated, this, &KlineSeriesModel::onKlineUpdated, Qt::UniqueConnection);
            connect(m_source, &KlineStore::allCleared, this, &KlineSeriesModel::onAllCleared, Qt::UniqueConnection);
        }

        refreshAll();
        emit klineStoreChanged();
    }

    void KlineSeriesModel::setTimeframe(const QString& timeframe)
    {
        if (m_timeframe == timeframe)
            return;

        m_timeframe = timeframe;
        refreshAll();
        emit timeframeChanged();
    }

    void KlineSeriesModel::refreshAll()
    {
        rebuildRows();
        beginResetModel();
        endResetModel();
        emit lengthChanged();
        emit modelUpdated();
    }

    void KlineSeriesModel::rebuildRows()
    {
        if (!m_source || m_timeframe.isEmpty()) {
            m_rows.clear();
            return;
        }
        m_rows = m_source->series(m_timeframe);
    }

    QVariantMap KlineSeriesModel::toMap(const Kline &k)
    {
        return {
            {"open",      k.m_open.toDouble()},
            {"close",     k.m_close.toDouble()},
            {"high",      k.m_high.toDouble()},
            {"low",       k.m_low.toDouble()},
            {"volume",    k.m_volume.toDouble()},
            {"turnover",  k.m_turnover.toDouble()},
            {"time",      k.m_start},
            {"isConfirm", k.m_confirm}
        };
    }

    void KlineSeriesModel::onSeriesReset(const QString& timeframe)
    {
        if (timeframe != m_timeframe)
            return;
        refreshAll();
    }

    void KlineSeriesModel::onAllCleared()
    {
        if (m_rows.isEmpty())
            return;

        beginResetModel();
        m_rows.clear();
        endResetModel();
        emit lengthChanged();
        emit modelUpdated();
    }

    void KlineSeriesModel::onKlineAppended(const QString& timeframe)
    {
        if (timeframe != m_timeframe || !m_source)
            return;

        Kline k;
        if (!m_source->lastKline(m_timeframe, k))
            return;

        const int row = m_rows.size();
        beginInsertRows(QModelIndex(), row, row);
        m_rows.append(k);
        endInsertRows();

        emit lengthChanged();
        emit modelUpdated();
    }

    void KlineSeriesModel::onKlineUpdated(const QString& timeframe)
    {
        if (timeframe != m_timeframe || !m_source || m_rows.isEmpty())
            return;

        Kline k;
        if (!m_source->lastKline(m_timeframe, k))
            return;

        m_rows.last() = k;

        const int row = m_rows.size() - 1;
        emit dataChanged(index(row, 0), index(row, 0),
                         {OpenRole, CloseRole, HighRole, LowRole,
                          VolumeRole, TurnoverRole, ConfirmRole});
        emit modelUpdated();
    }

    QVariantMap KlineSeriesModel::get(int idx) const
    {
        if (idx < 0 || idx >= m_rows.size())
            return {};
        return toMap(m_rows.at(idx));
    }

    QVariantMap KlineSeriesModel::last() const
    {
        return get(m_rows.size() - 1);
    }

    QVariantMap KlineSeriesModel::first() const
    {
        return get(0);
    }

}