#pragma once
#include "BaseMediator.hpp"
#include "Repository/AccountDataRepository.hpp"
#include "Service/IPrivateService.hpp"
#include "Streamer/IPrivateStreamer.hpp"
#include "Tools/ExportStructures.hpp"

namespace Core
{

class AccountMediator : public BaseMediator
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

    Tools::Orders readOrdersFromRepository(const Tools::StdExport& req);
    Tools::Executions readExecsFromRepository(const Tools::StdExport& req);

    void requestAccountBalance();
    void requestInfoAboutApi();

    void requestCreateOrder(const Tools::OrderRequest& request);
    void requestAmendOrder(const Tools::OrderAmendRequest& request);
    void requestCancelOrder(const Tools::OrderCancelRequest& request);
    void requestCancelAllOrders(const Tools::OrderCancelAllRequest& request);
    void requestOpenOrders(const Tools::OpenOrdersRequest& request);
    void requestOrderHistory(const Tools::OrderHistoryRequest& request);
    void requestTradeHistory(const Tools::TradeHistoryRequest& request);

    void init(const Tools::Api& api, bool isTestnet);

protected:

    Streamer::IStreamer* streamer() override { return m_privateStreamer.get(); }

signals:

    // Уведомляет о получении данных
    void accountVerificationReady(bool isValid = true);
    void accountBalanceReady(const Core::Tools::AccountBalance& balance);
    void apiReady(const Core::Tools::Api& api);
    void apiInfoReady(const Core::Tools::ApiInfo& apiInfo);
    void orderCreated(const QString& orderId, const QString& orderLinkId);
    void orderAmended(const QString& orderId, const QString& orderLinkId);
    void orderCancelled(const QString& orderId, const QString& orderLinkId);
    void allOrdersCancelled(const QList<QString>& cancelledOrderIds);
    void openOrdersReady(const QList<Core::Tools::OrderInfo>& orders);
    void orderHistoryReady(const QList<Core::Tools::OrderInfo>& orders);
    void tradeHistoryReady(const QList<Core::Tools::ExecutionInfo>& execs);
    void orderRequestRejected(const QString& orderLinkId, const QString& reason);

    void orderReady(const Core::Tools::OrderInfo& order);
    void executionReady(const Core::Tools::ExecutionInfo& exec);
    void positionReady(const Core::Tools::PositionInfo& position);

private slots:

    void onAccountVerificationReady();
    void onAccountBalanceReady(const Core::Tools::AccountBalance& balance);
    void onInfoAboutApiReady(const Core::Tools::ApiInfo& apiInfo);
    void onOrderCreated(const QString& orderId, const QString& orderLinkId);
    void onOrderAmended(const QString& orderId, const QString& orderLinkId);
    void onOrderCancelled(const QString& orderId, const QString& orderLinkId);
    void onAllOrdersCancelled(const QList<QString>& cancelledOrderIds);
    void onOpenOrdersReady(const QList<Core::Tools::OrderInfo>& orders);
    void onOrderHistoryReady(const QList<Core::Tools::OrderInfo>& orders);
    void onTradeHistoryReady(const QList<Core::Tools::ExecutionInfo>& execs);
    void onExecutionUpdated(const Core::Tools::ExecutionInfo& exec);
    void onOrderRequestRejected(const QString& orderLinkId, const QString& reason);

};

}