#ifndef WALLETCONTROLLER_HPP
#define WALLETCONTROLLER_HPP
#include <QAbstractListModel>
#include "Tools/StdTypes.hpp"
#include "AssetModel.hpp"

namespace Core::Tools
{

class WalletController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("WalletController connot be created in QML")

    Q_PROPERTY(AssetModel*  assets                     READ assets                     CONSTANT)
    Q_PROPERTY(QString      accountType                READ accountType                NOTIFY walletChanged FINAL)
    Q_PROPERTY(QString      totalWalletBalance         READ totalWalletBalance         NOTIFY walletChanged FINAL)
    Q_PROPERTY(QString      totalEquity                READ totalEquity                NOTIFY walletChanged FINAL)
    Q_PROPERTY(QString      totalMarginBalance         READ totalMarginBalance         NOTIFY walletChanged FINAL)
    Q_PROPERTY(QString      totalAvailableBalance      READ totalAvailableBalance      NOTIFY walletChanged FINAL)
    Q_PROPERTY(QString      totalPerpUPL               READ totalPerpUPL               NOTIFY walletChanged FINAL)
    Q_PROPERTY(QString      totalInitialMargin         READ totalInitialMargin         NOTIFY walletChanged FINAL)
    Q_PROPERTY(QString      totalMaintenanceMargin     READ totalMaintenanceMargin     NOTIFY walletChanged FINAL)
    Q_PROPERTY(QString      accountIMRate              READ accountIMRate              NOTIFY walletChanged FINAL)
    Q_PROPERTY(QString      accountMMRate              READ accountMMRate              NOTIFY walletChanged FINAL)
    Q_PROPERTY(QString      accountIMRateByMp          READ accountIMRateByMp          NOTIFY walletChanged FINAL)
    Q_PROPERTY(QString      accountMMRateByMp          READ accountMMRateByMp          NOTIFY walletChanged FINAL)
    Q_PROPERTY(QString      totalInitialMarginByMp     READ totalInitialMarginByMp     NOTIFY walletChanged FINAL)
    Q_PROPERTY(QString      totalMaintenanceMarginByMp READ totalMaintenanceMarginByMp NOTIFY walletChanged FINAL)
    Q_PROPERTY(qint64       creationTime               READ creationTime               NOTIFY walletChanged FINAL)

public:
    explicit WalletController(QObject* parent = nullptr);

    AssetModel* assets() { return &m_assets; }

    void updateWallet(const WalletInfo& wallet);
    Q_INVOKABLE void clear();

    QString accountType()                const { return m_info.accountType; }
    QString totalWalletBalance()         const { return m_info.totalWalletBalance; }
    QString totalEquity()                const { return m_info.totalEquity; }
    QString totalMarginBalance()         const { return m_info.totalMarginBalance; }
    QString totalAvailableBalance()      const { return m_info.totalAvailableBalance; }
    QString totalPerpUPL()               const { return m_info.totalPerpUPL; }
    QString totalInitialMargin()         const { return m_info.totalInitialMargin; }
    QString totalMaintenanceMargin()     const { return m_info.totalMaintenanceMargin; }
    QString accountIMRate()              const { return m_info.accountIMRate; }
    QString accountMMRate()              const { return m_info.accountMMRate; }
    QString accountIMRateByMp()          const { return m_info.accountIMRateByMp; }
    QString accountMMRateByMp()          const { return m_info.accountMMRateByMp; }
    QString totalInitialMarginByMp()     const { return m_info.totalInitialMarginByMp; }
    QString totalMaintenanceMarginByMp() const { return m_info.totalMaintenanceMarginByMp; }
    qint64  creationTime()               const { return m_info.creationTime; }

signals:
    void walletChanged();

private:
    bool updateAccountData(const WalletInfo& wallet);

    WalletInfo m_info;
    AssetModel m_assets;
};

}

#endif // WALLETCONTROLLER_HPP
