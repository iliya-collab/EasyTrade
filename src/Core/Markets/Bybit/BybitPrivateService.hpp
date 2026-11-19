#pragma once
#include "Service/BasePrivateService.hpp"

namespace Core
{

class BybitPrivateService : public Service::BasePrivateService
{
    Q_OBJECT
public:

    explicit BybitPrivateService(QObject* parent = nullptr);

    void init(const Api& api, bool isTestnet) override;

    void requestInfoAboutApi() override;
    void requestWalletBalance() override;
    void requestCreateOrder(const Tools::OrderRequest& request) override;
    void requestAmendOrder(const Tools::OrderAmendRequest& request) override;
    void requestCancelOrder(const Tools::OrderCancelRequest& request) override;
    void requestCancelAllOrders(const Tools::OrderCancelAllRequest& request) override;

    void requestFeeRates(const Tools::FeeRateRequest& request) override;
    void requestOpenOrdersAll(const Tools::OpenOrdersRequest& request) override;
    void requestOrderHistoryAll(const Tools::OrderHistoryRequest& request) override;
    void requestTradeHistoryAll(const Tools::TradeHistoryRequest& request) override;

private:

    void onOrderHistoryPageReceived(const Orders& orders, const QString& nextPageCursor, const QVariant& context) override;
    void onOpenOrdersPageReceived(const Orders& orders, const QString& nextPageCursor, const QVariant& context) override;
    void onTradeHistoryPageReceived(const Executions& execs, const QString& nextPageCursor, const QVariant& context) override;

};

}

