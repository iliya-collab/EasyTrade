#ifndef TRADEFILTERPROXYMODEL_HPP
#define TRADEFILTERPROXYMODEL_HPP

#include <QSortFilterProxyModel>
#include <qqmlregistration.h>

namespace Core::Tools {

class ExecutionFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString orderId READ orderId WRITE setOrderId NOTIFY orderIdChanged)

public:

    explicit ExecutionFilterProxyModel(QObject* parent = nullptr) : QSortFilterProxyModel(parent) {}

public:

    QString orderId() const { return m_orderId; }
    void setOrderId(const QString& orderId);

signals:

    void orderIdChanged();

protected:

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:

    QString m_orderId{};

};

}

#endif // TRADEFILTERPROXYMODEL_HPP
