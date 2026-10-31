#pragma once
#include "Service/BasePrivateService.hpp"

namespace Core
{

class BybitPrivateService : public Service::BasePrivateService
{
    Q_OBJECT
public:

    explicit BybitPrivateService(QObject* parent = nullptr);

    void init(const Tools::Api& api, bool isTestnet) override;

    void requestInfoAboutApi() override;
    void requestAccountBalance() override;
    void requestCreateOrder(const Tools::OrderRequest& request) override;
    void requestAmendOrder(const Tools::OrderAmendRequest& request) override;
    void requestCancelOrder(const Tools::OrderCancelRequest& request) override;
    void requestCancelAllOrders(const Tools::OrderCancelAllRequest& request) override;

    void requestOpenOrdersAll(const Tools::OpenOrdersRequest& request) override;
    void requestOrderHistoryAll(const Tools::OrderHistoryRequest& request) override;
    void requestTradeHistoryAll(const Tools::TradeHistoryRequest& request) override;

private:

    void onOrderHistoryPageReceived(const QList<Core::Tools::OrderInfo>& orders, const QString& nextPageCursor, const QVariant& context) override;
    void onOpenOrdersPageReceived(const QList<Core::Tools::OrderInfo>& orders, const QString& nextPageCursor, const QVariant& context) override;
    void onTradeHistoryPageReceived(const QList<Core::Tools::ExecutionInfo>& execs, const QString& nextPageCursor, const QVariant& context) override;

};

}

