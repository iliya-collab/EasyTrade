#pragma once
#include <QAbstractListModel>
#include "Tools/StdTypes.hpp"

namespace Core::Tools
{

    class AssetModel : public QAbstractListModel
    {
        Q_OBJECT

    public:

        enum AssetRoles {
            CoinRole = Qt::UserRole + 1,
            WalletBalanceRole,
            UsdValueRole,
            AssetRole
        };

        explicit AssetModel(QObject* parent = nullptr) : QAbstractListModel(parent) {};

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

        void upsertAsset(const AssetInfo& asset);
        void upsertAssets(const Assets& assets);

        Q_INVOKABLE AssetInfo asset(const QString& coin);
        Q_INVOKABLE void clear();

    private:

        Assets m_assets{};
        QHash<QString, int> m_nameAssetToRow{};

    };

}


