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
            NameRole = Qt::UserRole + 1,
            AmountRole,
            ValueRole
        };

        explicit AssetModel(QObject* parent = nullptr) : QAbstractListModel(parent) {};

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

        void upsertAsset(const Asset& asset);
        void upsertAssets(const std::vector<Asset>& assets);

    private:

        std::vector<Asset> m_assets{};
        QHash<QString, int> m_nameAssetToRow{};

    };

}


