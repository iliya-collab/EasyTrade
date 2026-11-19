#include "AssetModel.hpp"
#include <algorithm>

namespace Core::Tools
{

    int AssetModel::rowCount(const QModelIndex &parent) const
    {
        return m_assets.size();
    }

    QVariant AssetModel::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() || index.row() >= m_assets.size())
            return QVariant();

        const auto& asset = m_assets[index.row()];

        switch (role)
        {
        case CoinRole:
            return asset.coin;
        case WalletBalanceRole:
            return asset.walletBalance;
        case UsdValueRole:
            return asset.usdValue;
        case AssetRole:
            return QVariant::fromValue(asset);
        default:
            return QVariant();
        }

    }

    QHash<int, QByteArray> AssetModel::roleNames() const
    {
        return {
            {CoinRole, "coin"},
            {WalletBalanceRole, "walletBalance"},
            {UsdValueRole, "usdValue"},
            {AssetRole, "asset"}
        };
    }

    void AssetModel::upsertAsset(const AssetInfo &asset)
    {
        auto it = m_nameAssetToRow.find(asset.coin);

        if (it != m_nameAssetToRow.end())
        {
            int row = it.value();
            m_assets[row] = asset;
            const QModelIndex idx = index(row);
            emit dataChanged(idx, idx);
            return;
        }

        const int row = m_assets.size();
        beginInsertRows(QModelIndex(), row, row);
        m_assets.push_back(asset);
        m_nameAssetToRow.insert(asset.coin, row);
        endInsertRows();
    }

    void AssetModel::upsertAssets(const Assets& assets)
    {
        if (assets.empty())
            return;

        if (assets.size() == 1)
        {
            upsertAsset(assets.at(0));
            return;
        }

        std::vector<int> changedRows{};
        changedRows.reserve(assets.size());

        std::vector<AssetInfo> newAssets{};
        newAssets.reserve(assets.size());

        for (const auto& a : assets)
        {
            auto it = m_nameAssetToRow.find(a.coin);
            if (it != m_nameAssetToRow.end())
            {
                int row = it.value();
                m_assets[row] = a;
                changedRows.push_back(row);
            }
            else
            {
                auto it = find_if(newAssets.begin(), newAssets.end(),
                                  [&a](const AssetInfo& other) { return a.isSame(other); });
                if (it != newAssets.end())
                    *it = a;
                else
                    newAssets.push_back(a);
            }
        }

        if (!changedRows.empty())
        {
            std::sort(changedRows.begin(), changedRows.end());

            int first = changedRows[0];
            int last  = first;

            auto flush = [&]() {
                const QModelIndex tl = index(first, 0);
                const QModelIndex br = index(last, 0);
                emit dataChanged(tl, br);
            };

            for (int i = 1; i < changedRows.size(); ++i)
            {
                const int r = changedRows[i];
                if (r == last + 1)
                {
                    last = r;
                }
                else
                {
                    flush();
                    first = last = r;
                }
            }
            flush();
        }

        if (!newAssets.empty())
        {
            const int firstIdx = m_assets.size();
            const int lastIdx = m_assets.size() + newAssets.size() - 1;

            beginInsertRows(QModelIndex(), firstIdx, lastIdx);
            for (const auto& a : newAssets)
            {
                m_nameAssetToRow.insert(a.coin, m_assets.size());
                m_assets.push_back(a);
            }
            endInsertRows();
        }
    }

    AssetInfo AssetModel::asset(const QString &coin)
    {
        const int row = m_nameAssetToRow.value(coin, -1);
        return (row >= 0 && row < m_assets.size()) ? m_assets[row] : AssetInfo{};
    }

    void AssetModel::clear()
    {
        beginResetModel();
        m_assets.clear();
        m_nameAssetToRow.clear();
        endResetModel();
    }

}