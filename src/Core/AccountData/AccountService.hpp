#pragma once
#include "AccountMediator.hpp"
#include "AccountState.hpp"
#include "Core/BaseService.hpp"

namespace Core
{

class AccountService : public BaseService
{
    Q_OBJECT

private:

    std::shared_ptr<AccountState> m_state;
    std::shared_ptr<AccountMediator> m_mediator;
    bool m_enableTrades = false;

public:

    explicit AccountService(std::shared_ptr<AccountState> state,
                            std::shared_ptr<AccountMediator> mediator,
                            QObject* parent = nullptr);

    void setEnableTrades(bool enable) { m_enableTrades = enable; }
    bool getEnableTrades() { return m_enableTrades; }

    Q_INVOKABLE void run();
    Q_INVOKABLE void restart();
    Q_INVOKABLE void shutdown();
    Q_INVOKABLE void init(const Api& api, bool isTestnet);
    Q_INVOKABLE void subscribe();

    Q_INVOKABLE void requestWalletBalance();
    Q_INVOKABLE void requestInfoAboutApi();

    Q_INVOKABLE void createOrder(const QVariantMap& params);
    Q_INVOKABLE void amendOrder(const QVariantMap& params);
    Q_INVOKABLE void cancelOrder(const QVariantMap& params);
    Q_INVOKABLE void cancelAllOrders(const QVariantMap& params);

    Q_INVOKABLE void requestOpenOrders(const QVariantMap& params);
    Q_INVOKABLE void requestOrderHistory(const QVariantMap& params);
    Q_INVOKABLE void requestTradeHistory(const QVariantMap& params);
    Q_INVOKABLE void requestFeeRates(const QVariantMap& params);

    Q_INVOKABLE void exportOrders(const QString& path,
                                  Tools::Exporter::ExportFormat format,
                                  const QVariantMap& opts,
                                  const QVariantMap& params);

    Q_INVOKABLE void exportExecs(const QString& path,
                                  Tools::Exporter::ExportFormat format,
                                  const QVariantMap& opts,
                                  const QVariantMap& params);

private slots:

    void onErrorOccurredWithId(const QString& id, const QString& error);
    void onErrorOccurred(const QString& error);
    void onStreamerStarted(const QString& id);
    void onStreamerStopped(const QString& id);

};

}