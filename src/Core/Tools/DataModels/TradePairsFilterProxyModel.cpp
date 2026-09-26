#include "TradePairsFilterProxyModel.hpp"
#include "TradePairsModel.hpp"

namespace Core::Tools {

    TradePairsFilterProxyModel::TradePairsFilterProxyModel(QObject *parent)
        : QSortFilterProxyModel(parent)
    {
        setDynamicSortFilter(true);
        setFilterCaseSensitivity(Qt::CaseInsensitive);
    }

    void TradePairsFilterProxyModel::setSymbolFilter(const QString &text)
    {
        if (m_symbolFilter == text)
            return;

        m_symbolFilter = text;

        emit symbolFilterChanged();

        invalidate();
    }

    bool TradePairsFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
        if (m_symbolFilter.isEmpty())
            return true;

        auto idx = sourceModel()->index(sourceRow, 0, sourceParent);
        QString symbol = sourceModel()->data(idx, TradePairsModel::SymbolRole).toString();

        return symbol.contains(m_symbolFilter, Qt::CaseInsensitive);
    }

}

