#include "WalletController.hpp"

namespace Core::Tools
{

WalletController::WalletController(QObject* parent)
    : QObject(parent)
    , m_assets(this)
{}

bool WalletController::updateAccountData(const WalletInfo& wallet)
{
    const bool changed = !wallet.isSameAccountData(m_info)
                         || wallet.creationTime != m_info.creationTime;
    if (changed)
    {
        m_info = wallet;
        m_info.assets.clear();
    }
    return changed;
}

void WalletController::updateWallet(const WalletInfo& wallet)
{
    const bool changed = updateAccountData(wallet);
    m_assets.upsertAssets(wallet.assets);
    if (changed)
        emit walletChanged();
}

void WalletController::clear()
{
    m_assets.clear();
    if (m_info != WalletInfo{})
    {
        m_info = WalletInfo{};
        emit walletChanged();
    }
}

}