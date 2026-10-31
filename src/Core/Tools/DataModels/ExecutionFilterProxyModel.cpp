#include "ExecutionModel.hpp"
#include "ExecutionFilterProxyModel.hpp"

void Core::Tools::ExecutionFilterProxyModel::setOrderId(const QString &orderId)
{
    if (m_orderId == orderId)
        return;
    m_orderId = orderId;
    invalidate();
    emit orderIdChanged();
}

bool Core::Tools::ExecutionFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (m_orderId.isEmpty())
        return true;

    auto idx = sourceModel()->index(sourceRow, 0, sourceParent);
    const QString rowOrderId = sourceModel()->data(idx, ExecutionModel::OrderIdRole).toString();
    return rowOrderId == m_orderId;
}
