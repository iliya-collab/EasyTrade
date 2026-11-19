#pragma once
#include "Tools/Types/EnumHelpers.hpp"
#include <QSortFilterProxyModel>
#include <qqmlregistration.h>

namespace Core::Tools {

    class InstrumentFilterProxyModel : public QSortFilterProxyModel {
        Q_OBJECT
        QML_ELEMENT

        Q_PROPERTY(QString symbolFilter READ getSymbolFilter WRITE setSymbolFilter NOTIFY symbolFilterChanged FINAL)
        Q_PROPERTY(Types::EnumHelpers::MarketType categoryFilter READ getCategoryFilter WRITE setCategoryFilter NOTIFY categoryFilterChanged FINAL)

    public:

        explicit InstrumentFilterProxyModel(QObject* parent = nullptr);

        QString getSymbolFilter() const;
        Types::EnumHelpers::MarketType getCategoryFilter() const;

        void setSymbolFilter(const QString &text);
        void setCategoryFilter(Types::EnumHelpers::MarketType category);

    signals:
        void symbolFilterChanged();
        void categoryFilterChanged();

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

    private:

        QString m_symbolFilter = "";
        Types::EnumHelpers::MarketType m_categoryFilter;

    };

}


