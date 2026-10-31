#include "BybitPrivateService.hpp"
#include "Tools/PagedJob.hpp"
#include "Tools/Network/Bybit/BybitRestAPI.hpp"
#include "Handlers/Service/GetBybitAccountBalanceHandler.hpp"
#include "Handlers/Service/GetBybitInfoAboutAPIHandler.hpp"
#include "Handlers/Service/GetBybitOpenOrdersHandler.hpp"
#include "Handlers/Service/GetBybitOrderHistoryHandler.hpp"
#include "Handlers/Service/PostBybitCreateOrderHandler.hpp"
#include "Handlers/Service/PostBybitAmendOrderHandler.hpp"
#include "Handlers/Service/PostBybitCancelOrderHandler.hpp"
#include "Handlers/Service/PostBybitCancelAllOrdersHandler.hpp"
#include "Handlers/Service/GetBybitTradeHistoryHandler.hpp"

using OrderHistoryJob = Core::Tools::PagedJob<Core::Tools::OrderHistoryRequest, Core::Tools::OrderInfo>;
using OpenOrdersJob = Core::Tools::PagedJob<Core::Tools::OpenOrdersRequest, Core::Tools::OrderInfo>;
using TradeHistoryJob = Core::Tools::PagedJob<Core::Tools::TradeHistoryRequest, Core::Tools::ExecutionInfo>;

Core::BybitPrivateService::BybitPrivateService(QObject *parent)
    : BasePrivateService(std::make_unique<Tools::BybitRestAPI>(parent), parent)
{
    registerHandler<Handlers::GetBybitInfoAboutAPIHandler>();
    registerHandler<Handlers::GetBybitAccountBalanceHandler>();
    registerHandler<Handlers::GetBybitOpenOrdersHandler>();
    registerHandler<Handlers::GetBybitOrderHistoryHandler>();
    registerHandler<Handlers::PostBybitCreateOrderHandler>();
    registerHandler<Handlers::PostBybitAmendOrderHandler>();
    registerHandler<Handlers::PostBybitCancelOrderHandler>();
    registerHandler<Handlers::PostBybitCancelAllOrdersHandler>();
    registerHandler<Handlers::GetBybitTradeHistoryHandler>();
}

void Core::BybitPrivateService::requestInfoAboutApi()
{
    send<Handlers::GetBybitInfoAboutAPIHandler>(QVariant());
}

void Core::BybitPrivateService::requestAccountBalance()
{
    send<Handlers::GetBybitAccountBalanceHandler>(QVariant());
}

void Core::BybitPrivateService::init(const Tools::Api &api, bool isTestnet)
{
    if (!m_currentApi)
        return;
    m_currentApi->init(api, isTestnet);
}

void Core::BybitPrivateService::requestCreateOrder(const Tools::OrderRequest &request)
{
    send<Handlers::PostBybitCreateOrderHandler>(QVariant::fromValue(request), request);
}

void Core::BybitPrivateService::requestAmendOrder(const Tools::OrderAmendRequest &request)
{
    send<Handlers::PostBybitAmendOrderHandler>(QVariant::fromValue(request), request);
}

void Core::BybitPrivateService::requestCancelOrder(const Tools::OrderCancelRequest &request)
{
    send<Handlers::PostBybitCancelOrderHandler>(QVariant::fromValue(request), request);
}

void Core::BybitPrivateService::requestCancelAllOrders(const Tools::OrderCancelAllRequest &request)
{
    send<Handlers::PostBybitCancelAllOrdersHandler>(QVariant::fromValue(request), request);
}

void Core::BybitPrivateService::requestOpenOrdersAll(const Tools::OpenOrdersRequest &request)
{
    OpenOrdersJob job;
    job.setRequest(request);
    send<Handlers::GetBybitOpenOrdersHandler>(QVariant::fromValue(job), job.getRequest());
}

void Core::BybitPrivateService::requestOrderHistoryAll(const Tools::OrderHistoryRequest &request)
{
    OrderHistoryJob job;
    job.setRequest(request);
    send<Handlers::GetBybitOrderHistoryHandler>(QVariant::fromValue(job), job.getRequest());
}

void Core::BybitPrivateService::requestTradeHistoryAll(const Tools::TradeHistoryRequest &request)
{
    TradeHistoryJob job;
    job.setRequest(request);
    send<Handlers::GetBybitTradeHistoryHandler>(QVariant::fromValue(job), job.getRequest());
}

void Core::BybitPrivateService::onOrderHistoryPageReceived(const QList<Tools::OrderInfo> &orders, const QString &nextPageCursor, const QVariant& context)
{
    if (context.metaType() != QMetaType::fromType<OrderHistoryJob>())
    {
        emit signaller()->orderHistoryPageReceived(orders, nextPageCursor, context);
        return;
    }

    auto job = context.value<OrderHistoryJob>();
    if (job.addPage(orders, nextPageCursor))
        send<Handlers::GetBybitOrderHistoryHandler>(QVariant::fromValue(job), job.getRequest());
    else
        emit signaller()->allOrderHistoryReceived(job.get(), job.wasTruncated());
}

void Core::BybitPrivateService::onOpenOrdersPageReceived(const QList<Tools::OrderInfo> &orders, const QString &nextPageCursor, const QVariant& context)
{
    if (context.metaType() != QMetaType::fromType<OpenOrdersJob>())
    {
        emit signaller()->openOrdersPageReceived(orders, nextPageCursor, context);
        return;
    }

    auto job = context.value<OpenOrdersJob>();
    if (job.addPage(orders, nextPageCursor))
        send<Handlers::GetBybitOpenOrdersHandler>(QVariant::fromValue(job), job.getRequest());
    else
        emit signaller()->allOpenOrdersReceived(job.get(), job.wasTruncated());
}

void Core::BybitPrivateService::onTradeHistoryPageReceived(const QList<Tools::ExecutionInfo> &execs, const QString &nextPageCursor, const QVariant& context)
{
    if (context.metaType() != QMetaType::fromType<TradeHistoryJob>())
    {
        emit signaller()->tradeHistoryPageReceived(execs, nextPageCursor, context);
        return;
    }

    auto job = context.value<TradeHistoryJob>();
    if (job.addPage(execs, nextPageCursor))
        send<Handlers::GetBybitTradeHistoryHandler>(QVariant::fromValue(job), job.getRequest());
    else
        emit signaller()->allTradeHistoryReceived(job.get(), job.wasTruncated());
}

