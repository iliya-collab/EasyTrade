#include "ReversedProxyModel.hpp"

namespace Core::Tools {

    ReversedProxyModel::ReversedProxyModel(QObject* parent)
        : QAbstractProxyModel(parent) {}

    void ReversedProxyModel::setSourceModel(QAbstractItemModel* sourceModel)
    {
        if (sourceModel)
            disconnect(sourceModel, nullptr, this, nullptr);

        QAbstractProxyModel::setSourceModel(sourceModel);
        if (sourceModel)
        {
            connect(sourceModel, &QAbstractItemModel::rowsInserted,
                    this, &ReversedProxyModel::onRowsInserted);
            connect(sourceModel, &QAbstractItemModel::rowsRemoved,
                    this, &ReversedProxyModel::onRowsRemoved);
            connect(sourceModel, &QAbstractItemModel::dataChanged,
                    this, &ReversedProxyModel::onDataChanged);
            connect(sourceModel, &QAbstractItemModel::modelReset,
                    this, &ReversedProxyModel::onModelReset);
        }
    }

    void ReversedProxyModel::setReverse(bool rev)
    {
        if (m_reverse == rev)
            return;

        m_reverse = rev;

        beginResetModel();
        endResetModel();

        emit reverseChanged();
    }

    QModelIndex ReversedProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
    {
        if (!sourceIndex.isValid() || !sourceModel())
            return QModelIndex();
        int sourceRow = sourceIndex.row();
        int targetRow = m_reverse ? sourceModel()->rowCount() - 1 - sourceRow : sourceRow;
        return index(targetRow, sourceIndex.column());
    }

    QModelIndex ReversedProxyModel::mapToSource(const QModelIndex &proxyIndex) const
    {
        if (!proxyIndex.isValid() || !sourceModel())
            return QModelIndex();
        int targetRow = proxyIndex.row();
        int sourceRow = m_reverse ? sourceModel()->rowCount() - 1 - targetRow : targetRow;
        return sourceModel()->index(sourceRow, proxyIndex.column());
    }

    int ReversedProxyModel::rowCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent)
        return sourceModel() ? sourceModel()->rowCount() : 0;
    }

    int ReversedProxyModel::columnCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent)
        return sourceModel() ? sourceModel()->columnCount() : 0;
    }

    QModelIndex ReversedProxyModel::index(int row, int column, const QModelIndex &parent) const
    {
        Q_UNUSED(parent)
        return createIndex(row, column);
    }

    QModelIndex ReversedProxyModel::parent(const QModelIndex &child) const
    {
        Q_UNUSED(child)
        return QModelIndex();
    }

    QHash<int, QByteArray> ReversedProxyModel::roleNames() const
    {
        return sourceModel() ? sourceModel()->roleNames() : QHash<int, QByteArray>();
    }

    QVariant ReversedProxyModel::data(const QModelIndex &proxyIndex, int role) const
    {
        if (!proxyIndex.isValid() || !sourceModel())
            return QVariant();
        QModelIndex sourceIndex = mapToSource(proxyIndex);
        return sourceModel()->data(sourceIndex, role);
    }

    // ----- Обработка сигналов источника -----

    void ReversedProxyModel::onRowsInserted(const QModelIndex &parent, int first, int last)
    {
        if (parent.isValid())
            return;
        const int n = sourceModel()->rowCount();
        int pf = m_reverse ? (n - 1 - last)  : first;
        int pl = m_reverse ? (n - 1 - first) : last;
        beginInsertRows(QModelIndex(), pf, pl);
        endInsertRows();
    }

    void ReversedProxyModel::onRowsRemoved(const QModelIndex &parent, int first, int last)
    {
        if (parent.isValid())
            return;
        const int newN = sourceModel()->rowCount();
        const int oldN = newN + (last - first + 1);
        int pf = m_reverse ? (oldN - 1 - last)  : first;
        int pl = m_reverse ? (oldN - 1 - first) : last;
        beginRemoveRows(QModelIndex(), pf, pl);
        endRemoveRows();
    }

    void ReversedProxyModel::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
    {
        QModelIndex a = mapFromSource(topLeft);
        QModelIndex b = mapFromSource(bottomRight);
        if (m_reverse)
            std::swap(a, b);
        emit dataChanged(a, b, roles);
    }

    void ReversedProxyModel::onModelReset()
    {
        beginResetModel();
        endResetModel();
    }

}