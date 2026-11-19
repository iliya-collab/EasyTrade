#pragma once
#include "Tools/StdTypes.hpp"
#include <QAbstractListModel>
#include <QQueue>

namespace Core::Tools {

    class PublicTradesModel : public QAbstractListModel {
        Q_OBJECT
        QML_ELEMENT

    public:

        enum TradeRoles {
            TimeRole = Qt::UserRole + 1,
            PriceRole,
            VolumeRole,
            TurnoverRole,
            SideRole
        };

        explicit PublicTradesModel(QObject* parent = nullptr) : QAbstractListModel(parent) {};

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

        void addTrade(const PublicTradeInfo& item);
        void addTradeBatch(const PublicTrades& items);

    private:

        PublicTrades m_trades;
        int m_nTrades = 50;

    };

}
