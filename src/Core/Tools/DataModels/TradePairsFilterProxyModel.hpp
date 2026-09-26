#pragma once
#include <QSortFilterProxyModel>
#include <qqmlregistration.h>

namespace Core::Tools {

    class TradePairsFilterProxyModel : public QSortFilterProxyModel {
        Q_OBJECT
        QML_ELEMENT

        Q_PROPERTY(QString symbolFilter READ getSymbolFilter WRITE setSymbolFilter NOTIFY symbolFilterChanged FINAL)

    public:

        explicit TradePairsFilterProxyModel(QObject* parent = nullptr);

        QString getSymbolFilter() const { return m_symbolFilter; }
        void setSymbolFilter(const QString &text);

    signals:
        void symbolFilterChanged();

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

    private:

        QString m_symbolFilter = "";

    };

}


