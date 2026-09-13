#include "PublicTradesModel.hpp"

namespace Core::Tools {

    int PublicTradesModel::rowCount(const QModelIndex &parent) const
    {
        return m_trades.size();
    }

    QVariant PublicTradesModel::data(const QModelIndex &index, int role) const
    {

        if (!index.isValid() || index.row() >= m_trades.size())
            return QVariant();

        const auto& trade = m_trades.at(index.row());

        switch (role) {
            case TimeRole:
                return trade.m_tradeTime;
            case PriceRole:
                return trade.m_price;
            case VolumeRole:
                return trade.m_volume;
            case TurnoverRole:
                return trade.m_turnover;
            case SideRole:
                return QVariant::fromValue(trade.m_side);
            default:
                return QVariant();
        };

    }

    QHash<int, QByteArray> PublicTradesModel::roleNames() const
    {
        return {
            {TimeRole, "tradeTime"},
            {PriceRole, "tradePrice"},
            {VolumeRole, "tradeVolume"},
            {TurnoverRole, "tradeTurnover"},
            {SideRole, "tradeSide"}
        };
    }

    void PublicTradesModel::addTrade(const PublicTradeItem &item)
    {
        beginInsertRows(QModelIndex(), 0, 0);
        m_trades.prepend(item);
        endInsertRows();

        if (m_trades.size() > m_nTrades) {
            beginRemoveRows(QModelIndex(), m_trades.size() - 1, m_trades.size() - 1);
            m_trades.removeLast();
            endRemoveRows();
        }
    }

    void PublicTradesModel::addTradeBatch(const PublicTrades& items)
    {
        if (items.isEmpty())
            return;

        int count = items.size();
        int oldSize = m_trades.size();

        beginInsertRows(QModelIndex(), oldSize, oldSize + count - 1);
        for (const auto& item : items)
            m_trades.append(item);
        endInsertRows();

        if (m_trades.size() > m_nTrades) {
            int excess = m_trades.size() - m_nTrades;
            beginRemoveRows(QModelIndex(), 0, excess - 1);
            for (int i = 0; i < excess; ++i)
                m_trades.removeFirst();
            endRemoveRows();
        }
    }

}