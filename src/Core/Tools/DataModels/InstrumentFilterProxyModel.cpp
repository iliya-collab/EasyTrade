#include "InstrumentFilterProxyModel.hpp"
#include "InstrumentModel.hpp"

namespace Core::Tools {

    InstrumentFilterProxyModel::InstrumentFilterProxyModel(QObject *parent)
        : QSortFilterProxyModel(parent)
    {
        setDynamicSortFilter(true);
        setFilterCaseSensitivity(Qt::CaseInsensitive);
    }

    void InstrumentFilterProxyModel::setSymbolFilter(const QString &text)
    {
        if (m_symbolFilter == text)
            return;

        m_symbolFilter = text;

        emit symbolFilterChanged();

        invalidate();
    }

    bool InstrumentFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
    {
        if (m_symbolFilter.isEmpty())
            return true;

        auto idx = sourceModel()->index(sourceRow, 0, sourceParent);
        QString symbol = sourceModel()->data(idx, InstrumentModel::SymbolRole).toString();

        return symbol.contains(m_symbolFilter, Qt::CaseInsensitive);
    }

}

