#include "GetBybitWalletBalanceHandler.hpp"

void Core::Handlers::GetBybitWalletBalanceHandler::handle(const QJsonObject &data, const QVariant& requestContext, Service::PrivateServiceSignals* signaller)
{
    if (!data.contains("retCode") || data["retCode"].toInt() != 0)
    {
        emit signaller->errorOccurred(QString("Error processing request [endpoint = %1]: " + data["retMsg"].toString()).arg(endpoint()));
        return;
    }

    emit signaller->accountVerified();

    WalletInfo wallet{};

    const QJsonObject result = data["result"].toObject();
    const QJsonArray list = result["list"].toArray();

    if (list.isEmpty())
        return;

    const QJsonObject obj = list[0].toObject();

    // --- Account-level поля ---
    wallet.totalWalletBalance       = obj["totalWalletBalance"].toString();
    wallet.totalEquity              = obj["totalEquity"].toString();
    wallet.totalMarginBalance       = obj["totalMarginBalance"].toString();
    wallet.totalAvailableBalance    = obj["totalAvailableBalance"].toString();
    wallet.totalPerpUPL             = obj["totalPerpUPL"].toString();
    wallet.totalInitialMargin       = obj["totalInitialMargin"].toString();
    wallet.totalMaintenanceMargin   = obj["totalMaintenanceMargin"].toString();
    wallet.accountIMRate            = obj["accountIMRate"].toString();
    wallet.accountMMRate            = obj["accountMMRate"].toString();
    wallet.accountType              = obj["accountType"].toString();

    wallet.accountIMRateByMp           = obj["accountIMRateByMp"].toString();
    wallet.accountMMRateByMp           = obj["accountMMRateByMp"].toString();
    wallet.totalInitialMarginByMp      = obj["totalInitialMarginByMp"].toString();
    wallet.totalMaintenanceMarginByMp  = obj["totalMaintenanceMarginByMp"].toString();

    wallet.creationTime = data["time"].toInteger();

    // --- Coin-level поля ---
    const QJsonArray coins = obj["coin"].toArray();
    wallet.assets.reserve(coins.size());

    for (const auto& coinItem : std::as_const(coins))
    {
        const QJsonObject objCoin = coinItem.toObject();

        AssetInfo asset;
        asset.coin              = objCoin["coin"].toString();

        // Баланс
        asset.walletBalance     = objCoin["walletBalance"].toString();
        asset.equity            = objCoin["equity"].toString();
        asset.usdValue          = objCoin["usdValue"].toString();

        // P&L
        asset.unrealisedPnl     = objCoin["unrealisedPnl"].toString();
        asset.cumRealisedPnl    = objCoin["cumRealisedPnl"].toString();

        // Займы
        asset.spotBorrow        = objCoin["spotBorrow"].toString();
        asset.borrowAmount      = objCoin["borrowAmount"].toString();
        asset.accruedInterest   = objCoin["accruedInterest"].toString();

        // Маржа
        asset.totalOrderIM      = objCoin["totalOrderIM"].toString();
        asset.totalPositionIM   = objCoin["totalPositionIM"].toString();
        asset.totalPositionMM   = objCoin["totalPositionMM"].toString();

        // Прочее
        asset.locked            = objCoin["locked"].toString();
        asset.spotHedgingQty    = objCoin["spotHedgingQty"].toString();
        asset.bonus             = objCoin["bonus"].toString();
        asset.colRes            = objCoin["colRes"].toString();
        asset.marginCollateral  = objCoin["marginCollateral"].toBool();
        asset.collateralSwitch  = objCoin["collateralSwitch"].toBool();

        wallet.assets.emplace_back(std::move(asset));
    }

    emit signaller->walletReceived(wallet);
}