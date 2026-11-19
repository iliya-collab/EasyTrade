#ifndef WALLETINFO_HPP
#define WALLETINFO_HPP
#include <QString>
#include <QList>
#include "RegistrationToolsTypes.hpp"

namespace Core::Tools::Types
{

// ==========================================================================================
//  Информация об активах и кошельке
// ==========================================================================================

struct AssetInfo
{
    Q_GADGET
    QML_VALUE_TYPE(tAssetInfo)

    Q_PROPERTY(QString coin             READ getCoin FINAL)
    Q_PROPERTY(QString walletBalance    READ getWalletBalance FINAL)
    Q_PROPERTY(QString equity           READ getEquity FINAL)
    Q_PROPERTY(QString usdValue         READ getUsdValue FINAL)
    Q_PROPERTY(QString unrealisedPnl    READ getUnrealisedPnl FINAL)
    Q_PROPERTY(QString cumRealisedPnl   READ getCumRealisedPnl FINAL)
    Q_PROPERTY(QString spotBorrow       READ getSpotBorrow FINAL)
    Q_PROPERTY(QString borrowAmount     READ getBorrowAmount FINAL)
    Q_PROPERTY(QString accruedInterest  READ getAccruedInterest FINAL)
    Q_PROPERTY(QString totalOrderIM     READ getTotalOrderIM FINAL)
    Q_PROPERTY(QString totalPositionIM  READ getTotalPositionIM FINAL)
    Q_PROPERTY(QString totalPositionMM  READ getTotalPositionMM FINAL)
    Q_PROPERTY(QString locked           READ getLocked FINAL)
    Q_PROPERTY(QString spotHedgingQty   READ getSpotHedgingQty FINAL)
    Q_PROPERTY(QString bonus            READ getBonus FINAL)
    Q_PROPERTY(QString colRes           READ getColRes FINAL)
    Q_PROPERTY(bool    marginCollateral READ getMarginCollateral FINAL)
    Q_PROPERTY(bool    collateralSwitch READ getCollateralSwitch FINAL)

public:
    QString coin;                  // BTC, ETH, USDT, USDC ...

    // --- Баланс ---
    QString walletBalance;         // Кол-во актива в базовой монете
    QString equity;                // walletBalance - spotBorrow + unrealisedPnl
    QString usdValue;              // Цена актива в usd

    // --- P&L (для linear) ---
    QString unrealisedPnl;
    QString cumRealisedPnl;

    // --- Займы (spot margin) ---
    QString spotBorrow;            // только spot-займы
    QString borrowAmount;          // spot + derivatives liabilities
    QString accruedInterest;       // начисленные проценты

    // --- Маржа по монете (только для linear-позиций) ---
    QString totalOrderIM;          // зарезервировано под открытые ордера
    QString totalPositionIM;       // IM по открытым позициям
    QString totalPositionMM;       // MM по открытым позициям

    // --- Прочее ---
    QString locked;                 // заблокировано под spot-ордера
    QString spotHedgingQty;         // spot qty для хеджа в portfolio margin
    QString bonus;
    QString colRes;
    bool    marginCollateral = false;
    bool    collateralSwitch = false;

    // --- READ-методы ---
    QString getCoin()             const { return coin; }
    QString getWalletBalance()    const { return walletBalance; }
    QString getEquity()           const { return equity; }
    QString getUsdValue()         const { return usdValue; }
    QString getUnrealisedPnl()    const { return unrealisedPnl; }
    QString getCumRealisedPnl()   const { return cumRealisedPnl; }
    QString getSpotBorrow()       const { return spotBorrow; }
    QString getBorrowAmount()     const { return borrowAmount; }
    QString getAccruedInterest()  const { return accruedInterest; }
    QString getTotalOrderIM()     const { return totalOrderIM; }
    QString getTotalPositionIM()  const { return totalPositionIM; }
    QString getTotalPositionMM()  const { return totalPositionMM; }
    QString getLocked()           const { return locked; }
    QString getSpotHedgingQty()   const { return spotHedgingQty; }
    QString getBonus()            const { return bonus; }
    QString getColRes()           const { return colRes; }
    bool    getMarginCollateral() const { return marginCollateral; }
    bool    getCollateralSwitch() const { return collateralSwitch; }

    bool isSame(const AssetInfo& other) const { return coin == other.coin; }

    bool operator==(const AssetInfo& o) const {
        return coin                == o.coin
               && walletBalance    == o.walletBalance
               && equity           == o.equity
               && usdValue         == o.usdValue
               && unrealisedPnl    == o.unrealisedPnl
               && cumRealisedPnl   == o.cumRealisedPnl
               && spotBorrow       == o.spotBorrow
               && borrowAmount     == o.borrowAmount
               && accruedInterest  == o.accruedInterest
               && totalOrderIM     == o.totalOrderIM
               && totalPositionIM  == o.totalPositionIM
               && totalPositionMM  == o.totalPositionMM
               && locked           == o.locked
               && spotHedgingQty   == o.spotHedgingQty
               && bonus            == o.bonus
               && colRes           == o.colRes
               && marginCollateral == o.marginCollateral
               && collateralSwitch == o.collateralSwitch;
    }
    bool operator!=(const AssetInfo& o) const { return !(*this == o); }


};

using Assets = QList<AssetInfo>;

struct WalletInfo
{
    QString accountType;                    // UNIFIED

    // --- Балансы (USD) ---
    QString totalWalletBalance;             // сумма walletBalance
    QString totalEquity;                    // суммарный капитад (walletBalance - spotBorrow + UPL)
    QString totalMarginBalance;             // totalWalletBalance + totalPerpUPL
    QString totalAvailableBalance;          // доступно для новых ордеров
    QString totalPerpUPL;                   // UPL по linear (USDT/USDC перпы)

    // --- Маржа (USD) ---
    QString totalInitialMargin;
    QString totalMaintenanceMargin;

    // --- Коэффициенты риска ---
    QString accountIMRate;
    QString accountMMRate;

    // --- Дублирующие поля (ByMp) ---
    QString accountIMRateByMp;
    QString accountMMRateByMp;
    QString totalInitialMarginByMp;
    QString totalMaintenanceMarginByMp;

    // --- Активы ---
    Assets assets{};

    qint64 creationTime;

    bool isSameAccountData(const WalletInfo& o) const {
        return accountType                   == o.accountType
               && totalWalletBalance         == o.totalWalletBalance
               && totalEquity                == o.totalEquity
               && totalMarginBalance         == o.totalMarginBalance
               && totalAvailableBalance      == o.totalAvailableBalance
               && totalPerpUPL               == o.totalPerpUPL
               && totalInitialMargin         == o.totalInitialMargin
               && totalMaintenanceMargin     == o.totalMaintenanceMargin
               && accountIMRate              == o.accountIMRate
               && accountMMRate              == o.accountMMRate
               && accountIMRateByMp          == o.accountIMRateByMp
               && accountMMRateByMp          == o.accountMMRateByMp
               && totalInitialMarginByMp     == o.totalInitialMarginByMp
               && totalMaintenanceMarginByMp == o.totalMaintenanceMarginByMp;
    }

    bool operator==(const WalletInfo& o) const {
        return isSameAccountData(o) && assets == o.assets && creationTime == o.creationTime;
    }
    bool operator!=(const WalletInfo& o) const { return !(*this == o); }
};

}

#endif // WALLETINFO_HPP
