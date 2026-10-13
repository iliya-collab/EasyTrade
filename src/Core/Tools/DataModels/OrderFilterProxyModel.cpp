#include "OrderFilterProxyModel.hpp"
#include "OrderModel.hpp"

Core::Tools::OrderFilterProxyModel::OrderFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void Core::Tools::OrderFilterProxyModel::setStatuses(OrderStatuses statuses)
{
    if (m_statuses == statuses)
        return;

    m_statuses = statuses;

    emit statusesChanged();
    invalidate();
}

bool Core::Tools::OrderFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (m_statuses.isEmpty())
        return false;

    auto idx = sourceModel()->index(sourceRow, 0, sourceParent);
    QString stat = sourceModel()->data(idx, OrderModel::StatusRole).toString();

    return m_statuses.contains(EnumHelpers::stringToOrderStatus(stat));
}
