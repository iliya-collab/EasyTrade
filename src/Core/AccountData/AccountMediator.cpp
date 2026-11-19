#include "AccountMediator.hpp"
#include "Markets/Bybit/BybitPrivateService.hpp"
#include "Markets/Bybit/BybitPrivateStreamer.hpp"
#include "Tools/Logging.hpp"

namespace Core {

AccountMediator::AccountMediator(QObject* parent) : SingleMediator(parent)
{
    m_privateService    = std::make_unique<BybitPrivateService>(this);
    m_privateStreamer   = std::make_unique<BybitPrivateStreamer>(this);
    m_accountDataRep    = std::make_unique<Repository::AccountDataRepository>(this);

    bindService(m_privateService->signaller());
    bindStreamer(m_privateStreamer->signaller());

    // Связываем PrivateService
    connect(m_privateService->signaller(), &Service::PrivateServiceSignals::accountVerified,
            this, &AccountMediator::onAccountVerificationReady);

    connect(m_privateService->signaller(), &Service::PrivateServiceSignals::walletReceived,
            this, &AccountMediator::onWalletSnapshotReady);

    connect(m_privateService->signaller(), &Service::PrivateServiceSignals::infoAboutApiReceived,
            this, &AccountMediator::onInfoAboutApiReady);

    connect(m_privateService->signaller(), &Service::PrivateServiceSignals::orderCreated,
            this, &AccountMediator::onOrderCreated);

    connect(m_privateService->signaller(), &Service::PrivateServiceSignals::orderAmended,
            this, &AccountMediator::onOrderAmended);

    connect(m_privateService->signaller(), &Service::PrivateServiceSignals::orderCancelled,
            this, &AccountMediator::onOrderCancelled);

    connect(m_privateService->signaller(), &Service::PrivateServiceSignals::allOrdersCancelled,
            this, &AccountMediator::onAllOrdersCancelled);

    connect(m_privateService->signaller(), &Service::PrivateServiceSignals::allOpenOrdersReceived,
            this, &AccountMediator::onOpenOrdersReady);

    connect(m_privateService->signaller(), &Service::PrivateServiceSignals::allOrderHistoryReceived,
            this, &AccountMediator::onOrderHistoryReady);

    connect(m_privateService->signaller(), &Service::PrivateServiceSignals::allTradeHistoryReceived,
            this, &AccountMediator::onTradeHistoryReady);

    connect(m_privateService->signaller(), &Service::PrivateServiceSignals::feeRatesReceived,
            this, &AccountMediator::onFeeRatesReady);

    connect(m_privateService->signaller(), &Service::PrivateServiceSignals::orderRequestRejected,
            this, &AccountMediator::onOrderRequestRejected);

    // Связываем PrivateStreamer
    connect(m_privateStreamer->signaller(), &Streamer::PrivateStreamerSignals::orderUpdated,
            this, &AccountMediator::orderReady);

    connect(m_privateStreamer->signaller(), &Streamer::PrivateStreamerSignals::executionUpdated,
            this, &AccountMediator::onExecutionUpdated);

    connect(m_privateStreamer->signaller(), &Streamer::PrivateStreamerSignals::positionUpdated,
            this, &AccountMediator::positionReady);

    connect(m_privateStreamer->signaller(), &Streamer::PrivateStreamerSignals::walletUpdated,
            this, &AccountMediator::onWalletReady);

    connect(m_accountDataRep.get(), & Repository::AccountDataRepository::errorOccurred,
            this, &AccountMediator::errorOccurred);

}

void AccountMediator::subscribe()
{
    m_privateStreamer->subscribe({
        Streamer::PrivateStreams::Wallet,
        Streamer::PrivateStreams::Order,
        Streamer::PrivateStreams::Execution,
        Streamer::PrivateStreams::Position
    });
}

Orders AccountMediator::readOrdersFromRepository(const Tools::StdExport& req)
{
    return m_accountDataRep->loadOrders(req.m_category, req.m_symbol);
}

Executions AccountMediator::readExecsFromRepository(const Tools::StdExport &req)
{
    return m_accountDataRep->loadExecs(req.m_category, req.m_symbol);
}

void AccountMediator::requestWalletBalance()
{
    emit messageSent("Loading wallet balance...");
    m_privateService->requestWalletBalance();
}

void AccountMediator::requestInfoAboutApi()
{
    emit messageSent("Loading information about your API...");
    m_privateService->requestInfoAboutApi();
}

void AccountMediator::requestCreateOrder(const Tools::OrderRequest &request)
{
    emit messageSent("Creating order...");
    m_privateService->requestCreateOrder(request);
}

void AccountMediator::requestAmendOrder(const Tools::OrderAmendRequest &request)
{
    emit messageSent("Amending order...");
    m_privateService->requestAmendOrder(request);
}

void AccountMediator::requestCancelOrder(const Tools::OrderCancelRequest &request)
{
    emit messageSent("Order cancellation...");
    m_privateService->requestCancelOrder(request);
}

void AccountMediator::requestCancelAllOrders(const Tools::OrderCancelAllRequest &request)
{
    emit messageSent("Cancellation of all orders...");
    m_privateService->requestCancelAllOrders(request);
}

void AccountMediator::requestOpenOrders(const Tools::OpenOrdersRequest &request)
{
    emit messageSent("Loading open orders...");
    m_privateService->requestOpenOrdersAll(request);
}

void AccountMediator::requestOrderHistory(const Tools::OrderHistoryRequest &request)
{
    emit messageSent("Loading history orders...");
    m_privateService->requestOrderHistoryAll(request);
}

void AccountMediator::requestTradeHistory(const Tools::TradeHistoryRequest &request)
{
    emit messageSent("Loading history trades...");
    m_privateService->requestTradeHistoryAll(request);
}

void AccountMediator::requestFeeRates(const Tools::FeeRateRequest &request)
{
    emit messageSent("Loading fee rates...");
    m_privateService->requestFeeRates(request);
}

void AccountMediator::init(const Api &api, bool isTestnet)
{
    m_privateService->init(api, isTestnet);
    m_privateStreamer->init(api, isTestnet);
    m_testnet = isTestnet;
    emit apiReady(api);
}

void AccountMediator::onAccountVerificationReady()
{
    emit messageSent("Account verified");
    emit accountVerificationReady();
}

void AccountMediator::onWalletReady(const WalletInfo& wallet)
{
    emit messageSent("Wallet updated");
    emit walletReady(wallet);
}

void AccountMediator::onWalletSnapshotReady(const WalletInfo& wallet)
{
    emit messageSent("Wallet loaded");
    emit walletSnapshotReady(wallet);
}

void AccountMediator::onFeeRatesReady(const FeeRatesInfo &rates)
{
    emit messageSent("Fee rates ready");
    emit feeRatesReady(rates);
}

void AccountMediator::onInfoAboutApiReady(const ApiInfo &apiInfo)
{
    emit messageSent("The information about your API ready");
    try {
        m_accountDataRep->init(apiInfo.userId, m_testnet);
    } catch (const std::exception& e) {
        emit errorOccurred(e.what());
        return;
    }
    emit apiInfoReady(apiInfo);
}

void AccountMediator::onOrderCreated(const QString &orderId, const QString &orderLinkId)
{
    emit messageSent("The order [" + orderLinkId + "] was created");
    emit orderCreated(orderId, orderLinkId);
}

void AccountMediator::onOrderAmended(const QString &orderId, const QString &orderLinkId)
{
    emit messageSent("The order [" + orderLinkId + "] was amended");
    emit orderAmended(orderId, orderLinkId);
}

void AccountMediator::onOrderCancelled(const QString &orderId, const QString &orderLinkId)
{
    emit messageSent("The order [" + orderLinkId + "] was cancelled");
    emit orderCancelled(orderId, orderLinkId);
}

void AccountMediator::onAllOrdersCancelled(const QList<QString> &cancelledOrderIds)
{
    emit messageSent("All orders were cancelled");
    emit allOrdersCancelled(cancelledOrderIds);
}

void AccountMediator::onExecutionUpdated(const ExecutionInfo &exec)
{
    m_accountDataRep->saveExec(exec);
    emit executionReady(exec);
}

void AccountMediator::onTradeHistoryReady(const Executions &execs)
{
    emit messageSent(QString("The trade history is ready. %1 execution were received.").arg(execs.size()));
    m_accountDataRep->saveExecs(execs);
    emit tradeHistoryReady(execs);
}

void AccountMediator::onOrderRequestRejected(const QString &orderLinkId, const QString &reason)
{
    emit errorOccurred("The order [" + orderLinkId + "] was rejected. Reason : " + reason);
    emit orderRequestRejected(orderLinkId, reason);
}

void AccountMediator::onOrderHistoryReady(const Orders &orders)
{
    emit messageSent(QString("The order history is ready. %1 orders were received.").arg(orders.size()));
    m_accountDataRep->saveOrders(orders);
    emit orderHistoryReady(orders);
}

void AccountMediator::onOpenOrdersReady(const Orders &orders)
{
    emit messageSent(QString("The opened orders is ready. %1 orders were received.").arg(orders.size()));
    m_accountDataRep->saveOrders(orders);
    emit openOrdersReady(orders);
}

}