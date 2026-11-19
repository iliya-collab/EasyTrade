#include "BybitWalletStreamHandler.hpp"

void Core::Handlers::BybitWalletStreamHandler::handle(const QString& id, const QJsonObject &data, Streamer::PrivateStreamerSignals *signaller)
{
    if (!data.contains("data") || !data["data"].isArray())
    {
        emit signaller->errorOccurred(id, "[" + topic() + "] Invalid response structure!");
        return;
    }

    const QJsonArray dataArray = data["data"].toArray();
    if (dataArray.isEmpty())
        return;

    const QJsonObject dataObj = dataArray.at(0).toObject();

    WalletInfo wallet{};

    // --- Account-level поля ---
    wallet.accountType              = dataObj["accountType"].toString();

    wallet.totalWalletBalance       = dataObj["totalWalletBalance"].toString();
    wallet.totalEquity              = dataObj["totalEquity"].toString();
    wallet.totalMarginBalance       = dataObj["totalMarginBalance"].toString();
    wallet.totalAvailableBalance    = dataObj["totalAvailableBalance"].toString();
    wallet.totalPerpUPL             = dataObj["totalPerpUPL"].toString();

    wallet.totalInitialMargin       = dataObj["totalInitialMargin"].toString();
    wallet.totalMaintenanceMargin   = dataObj["totalMaintenanceMargin"].toString();

    wallet.accountIMRate            = dataObj["accountIMRate"].toString();
    wallet.accountMMRate            = dataObj["accountMMRate"].toString();

    wallet.accountIMRateByMp           = dataObj["accountIMRateByMp"].toString();
    wallet.accountMMRateByMp           = dataObj["accountMMRateByMp"].toString();
    wallet.totalInitialMarginByMp      = dataObj["totalInitialMarginByMp"].toString();
    wallet.totalMaintenanceMarginByMp  = dataObj["totalMaintenanceMarginByMp"].toString();

    wallet.creationTime     = data["creationTime"].toInteger();

    // --- Coin-level поля ---
    const QJsonArray coins = dataObj["coin"].toArray();
    wallet.assets.reserve(coins.size());

    for (const auto& coin : std::as_const(coins))
    {
        const QJsonObject objCoin = coin.toObject();

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

    emit signaller->walletUpdated(wallet);
}
