#include "AccountService.hpp"
#include "Tools/RequestBuilders.hpp"
#include "Tools/ExportBuilders.hpp"

Core::AccountService::AccountService(std::shared_ptr<AccountState> state, std::shared_ptr<AccountMediator> mediator, QObject *parent)
    : m_state(state), m_mediator(mediator), QObject(parent)
{
    connect(mediator.get(), &AccountMediator::apiReady,
            state.get(), &AccountState::updateApi);

    connect(mediator.get(), &AccountMediator::accountVerificationReady,
            state.get(), &AccountState::updateValidAccount);

    connect(mediator.get(), &AccountMediator::apiInfoReady,
            state.get(), &AccountState::updateApiInfo);

    connect(mediator.get(), &AccountMediator::accountBalanceReady,
            state.get(), &AccountState::upsertBalance);

    connect(mediator.get(), &AccountMediator::orderReady,
            state.get(), &AccountState::updateOrder);

    connect(mediator.get(), &AccountMediator::executionReady,
            state.get(), &AccountState::updateExecution);

    connect(mediator.get(), &AccountMediator::positionReady,
            state.get(), &AccountState::updatePosition);

    connect(mediator.get(), &AccountMediator::orderHistoryReady,
            state.get(), &AccountState::updateOrders);

    connect(mediator.get(), &AccountMediator::openOrdersReady,
            state.get(), &AccountState::updateOrders);

    connect(mediator.get(), &AccountMediator::tradeHistoryReady,
            state.get(), &AccountState::updateExecutions);

    connect(mediator.get(), &AccountMediator::messageSent,
            this, &AccountService::messageReceived);

    connect(mediator.get(), &AccountMediator::streamerStarted,
            this, &AccountService::onStreamerStarted);

    connect(mediator.get(), &AccountMediator::streamerStopped,
            this, &AccountService::onStreamerStopped);

    connect(mediator.get(), &AccountMediator::downloadProgress,
            this, &AccountService::downloadProgress);

    connect(mediator.get(), &AccountMediator::errorOccurredWithId,
            this, &AccountService::onErrorOccurredWithId);

    connect(mediator.get(), &AccountMediator::errorOccurred,
            this, &AccountService::onErrorOccurred);
}

void Core::AccountService::init(const Tools::Api &api, bool isTestnet)
{
    m_mediator->init(api, isTestnet);
}

void Core::AccountService::run()
{
    m_mediator->runStreamer();
}

void Core::AccountService::restart()
{
    m_mediator->restartStreamer();
}

void Core::AccountService::shutdown()
{
    m_mediator->stopStreamer();

    QEventLoop loop;
    QTimer::singleShot(5000, &loop, &QEventLoop::quit);

    connect(m_mediator.get(), &Core::AccountMediator::streamerStopped,
            &loop, &QEventLoop::quit, Qt::SingleShotConnection);

    loop.exec();
}

void Core::AccountService::subscribe()
{
    m_mediator->subscribe();
}

void Core::AccountService::requestAccountBalance()
{
    m_mediator->requestAccountBalance();
}

void Core::AccountService::requestInfoAboutApi()
{
    m_mediator->requestInfoAboutApi();
}

void Core::AccountService::onErrorOccurredWithId(const QString& id, const QString &error)
{
    QString errMsg = id + " : " + error;
    emit errorOccurred(errMsg);
}

void Core::AccountService::onErrorOccurred(const QString &error)
{
    emit errorOccurred(error);
}

void Core::AccountService::onStreamerStarted(const QString& id)
{
    subscribe();
    emit streamerStarted(id);
}

void Core::AccountService::onStreamerStopped(const QString& id)
{
    emit streamerStopped(id);
}

void Core::AccountService::createOrder(const QVariantMap& params)
{
    if (!m_enableTrades)
        return;

    validateAndSend(params, Tools::buildOrderRequest,
                    [this](const Tools::OrderRequest& req) { m_mediator->requestCreateOrder(req); });
}

void Core::AccountService::amendOrder(const QVariantMap& params)
{
    if (!m_enableTrades)
        return;

    validateAndSend(params, Tools::buildOrderAmendRequest,
                    [this](const Tools::OrderAmendRequest& req) { m_mediator->requestAmendOrder(req); });
}

void Core::AccountService::cancelOrder(const QVariantMap& params)
{
    if (!m_enableTrades)
        return;

    validateAndSend(params, Tools::buildOrderCancelRequest,
                    [this](const Tools::OrderCancelRequest& req) { m_mediator->requestCancelOrder(req); });
}

void Core::AccountService::cancelAllOrders(const QVariantMap& params)
{
    if (!m_enableTrades)
        return;

    validateAndSend(params, Tools::buildOrderCancelAllRequest,
                    [this](const Tools::OrderCancelAllRequest& req) { m_mediator->requestCancelAllOrders(req); });
}

void Core::AccountService::requestOpenOrders(const QVariantMap& params)
{
    validateAndSend(params, Tools::buildOpenOrdersRequest,
                    [this](const Tools::OpenOrdersRequest& req) { m_mediator->requestOpenOrders(req); });
}

void Core::AccountService::requestOrderHistory(const QVariantMap& params)
{
    validateAndSend(params, Tools::buildOrderHistoryRequest,
                    [this](const Tools::OrderHistoryRequest& req) { m_mediator->requestOrderHistory(req); });
}

void Core::AccountService::requestTradeHistory(const QVariantMap &params)
{
    validateAndSend(params, Tools::buildTradeHistoryRequest,
                    [this](const Tools::TradeHistoryRequest& req) { m_mediator->requestTradeHistory(req); });
}

void Core::AccountService::exportOrders(const QString &path, Tools::Exporter::ExportFormat format, const QVariantMap &opts, const QVariantMap &params)
{
    auto req = Tools::buildStdExport(params);
    if (!req.has_value())
    {
        emit errorOccurred(req.error());
        return;
    }

    auto orders = m_mediator->readOrdersFromRepository(req.value());
    if (orders.isEmpty())
    {
        emit errorOccurred("export: no orders in repository");
        return;
    }

    const QUrl url(path);
    const QString localPath = url.isLocalFile() ? url.toLocalFile() : path;

    doExport(orders, format, localPath, [](const Tools::OrderInfo& o) { return Tools::Exporter::toField(o); }, opts);
}

void Core::AccountService::exportExecs(const QString &path, Tools::Exporter::ExportFormat format, const QVariantMap &opts, const QVariantMap &params)
{
    auto req = Tools::buildStdExport(params);
    if (!req.has_value())
    {
        emit errorOccurred(req.error());
        return;
    }

    auto execs = m_mediator->readExecsFromRepository(req.value());
    if (execs.isEmpty())
    {
        emit errorOccurred("export: no execs in repository");
        return;
    }

    const QUrl url(path);
    const QString localPath = url.isLocalFile() ? url.toLocalFile() : path;

    doExport(execs, format, localPath, [](const Tools::ExecutionInfo& e) { return Tools::Exporter::toField(e); }, opts);
}
