#include "InstrumentFilterProxyModel.hpp"
#include "InstrumentModel.hpp"

namespace Core::Tools {

    InstrumentFilterProxyModel::InstrumentFilterProxyModel(QObject *parent)
        : QSortFilterProxyModel(parent)
    {
        setDynamicSortFilter(true);
        setFilterCaseSensitivity(Qt::CaseInsensitive);
    }

    QString InstrumentFilterProxyModel::getSymbolFilter() const { return m_symbolFilter; }

    Types::EnumHelpers::MarketType InstrumentFilterProxyModel::getCategoryFilter() const
    {
        return m_categoryFilter;
    }

    void InstrumentFilterProxyModel::setSymbolFilter(const QString &text)
    {
        if (m_symbolFilter == text)
            return;

        m_symbolFilter = text;

        emit symbolFilterChanged();

        invalidate();
    }

    void InstrumentFilterProxyModel::setCategoryFilter(Types::EnumHelpers::MarketType category)
    {
        if (m_categoryFilter == category)
            return;

        m_categoryFilter = category;

        emit categoryFilterChanged();

        invalidate();
    }

    bool InstrumentFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
    {
        const auto *model = sourceModel();
        if (!model)
            return true;

        const QModelIndex idx = model->index(sourceRow, 0, sourceParent);

        if (!m_symbolFilter.isEmpty())
        {
            const QString symbol = model->data(idx, InstrumentModel::SymbolRole).toString();
            if (!symbol.contains(m_symbolFilter, Qt::CaseInsensitive))
                return false;
        }

        if (m_categoryFilter != EnumHelpers::MarketType::Unknown)
        {
            EnumHelpers::MarketType category = model->data(idx, InstrumentModel::CategoryRole).value<EnumHelpers::MarketType>();
            if (category != m_categoryFilter)
                return false;
        }

        return true;
    }

}

