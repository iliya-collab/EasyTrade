#pragma once
#include "SingleMediator.hpp"
#include "Repository/AccountDataRepository.hpp"
#include "Service/IPrivateService.hpp"
#include "Streamer/IPrivateStreamer.hpp"
#include "Tools/ExportStructures.hpp"

namespace Core
{

class AccountMediator : public SingleMediator
{
    Q_OBJECT
private:

    std::unique_ptr<Service::IPrivateService> m_privateService{};
    std::unique_ptr<Streamer::IPrivateStreamer> m_privateStreamer{};
    std::unique_ptr<Repository::AccountDataRepository> m_accountDataRep{};
    bool m_testnet{};

public:

    explicit AccountMediator(QObject* parent = nullptr);

    void subscribe();

    Orders readOrdersFromRepository(const Tools::StdExport& req);
    Executions readExecsFromRepository(const Tools::StdExport& req);

    void requestWalletBalance();
    void requestInfoAboutApi();

    void requestCreateOrder(const Tools::OrderRequest& request);
    void requestAmendOrder(const Tools::OrderAmendRequest& request);
    void requestCancelOrder(const Tools::OrderCancelRequest& request);
    void requestCancelAllOrders(const Tools::OrderCancelAllRequest& request);
    void requestOpenOrders(const Tools::OpenOrdersRequest& request);
    void requestOrderHistory(const Tools::OrderHistoryRequest& request);
    void requestTradeHistory(const Tools::TradeHistoryRequest& request);
    void requestFeeRates(const Tools::FeeRateRequest& request);

    void init(const Api& api, bool isTestnet);

protected:

    Streamer::IStreamer* streamer() override { return m_privateStreamer.get(); }

signals:

    // Уведомляет о получении данных
    void accountVerificationReady();
    void walletSnapshotReady(const WalletInfo& wallet);
    void apiReady(const Api& api);
    void apiInfoReady(const ApiInfo& apiInfo);
    void feeRatesReady(const FeeRatesInfo& rates);
    void orderCreated(const QString& orderId, const QString& orderLinkId);
    void orderAmended(const QString& orderId, const QString& orderLinkId);
    void orderCancelled(const QString& orderId, const QString& orderLinkId);
    void allOrdersCancelled(const QList<QString>& cancelledOrderIds);
    void openOrdersReady(const QList<OrderInfo>& orders);
    void orderHistoryReady(const QList<OrderInfo>& orders);
    void tradeHistoryReady(const QList<ExecutionInfo>& execs);
    void orderRequestRejected(const QString& orderLinkId, const QString& reason);

    void walletReady(const WalletInfo& wallet);
    void orderReady(const OrderInfo& order);
    void executionReady(const ExecutionInfo& exec);
    void positionReady(const PositionInfo& position);

private slots:

    void onAccountVerificationReady();
    void onWalletReady(const WalletInfo& wallet);
    void onWalletSnapshotReady(const WalletInfo& wallet);
    void onFeeRatesReady(const FeeRatesInfo& rates);
    void onInfoAboutApiReady(const ApiInfo& apiInfo);
    void onOrderCreated(const QString& orderId, const QString& orderLinkId);
    void onOrderAmended(const QString& orderId, const QString& orderLinkId);
    void onOrderCancelled(const QString& orderId, const QString& orderLinkId);
    void onAllOrdersCancelled(const QList<QString>& cancelledOrderIds);
    void onOpenOrdersReady(const QList<OrderInfo>& orders);
    void onOrderHistoryReady(const QList<OrderInfo>& orders);
    void onTradeHistoryReady(const QList<ExecutionInfo>& execs);
    void onExecutionUpdated(const ExecutionInfo& exec);
    void onOrderRequestRejected(const QString& orderLinkId, const QString& reason);

};

}