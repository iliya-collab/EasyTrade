#ifndef ORDERFILTERPROXYMODEL_HPP
#define ORDERFILTERPROXYMODEL_HPP

#include <QSortFilterProxyModel>
#include <qqmlregistration.h>
#include "Tools/EnumHelpers.hpp"

namespace Core::Tools {

class OrderFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QList<Tools::EnumHelpers::OrderStatus> statuses READ getStatuses WRITE setStatuses NOTIFY statusesChanged FINAL)

public:

    using OrderStatuses = QList<Tools::EnumHelpers::OrderStatus>;

    explicit OrderFilterProxyModel(QObject* parent = nullptr);

    void setStatuses(OrderStatuses statuses);
    OrderStatuses getStatuses() const { return m_statuses; }

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

signals:
    void statusesChanged();

private:
    OrderStatuses m_statuses{};

};

}

#endif
