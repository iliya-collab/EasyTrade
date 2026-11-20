#include "AccountService.hpp"
#include "Tools/RequestBuilders.hpp"
#include "Tools/ExportBuilders.hpp"

Core::AccountService::AccountService(std::shared_ptr<AccountState> state, std::shared_ptr<AccountMediator> mediator, QObject *parent)
    : m_state(state), m_mediator(mediator), BaseService(parent)
{

    connect(mediator.get(), &AccountMediator::accountVerificationReady,
            state.get(), &AccountState::setAccountVerified);

    connect(mediator.get(), &AccountMediator::apiReady,
            state.get(), &AccountState::setApi);

    connect(mediator.get(), &AccountMediator::apiInfoReady,
            state.get(), &AccountState::setApiInfo);

    connect(mediator.get(), &AccountMediator::walletReady,
            state.get(), &AccountState::upsertWallet);

    connect(mediator.get(), &AccountMediator::walletSnapshotReady,
            state.get(), &AccountState::upsertWallet);

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

    connect(mediator.get(), &AccountMediator::feeRatesReady,
            state.get(), &AccountState::setFeeRates);

// ---------------------------------------------------------------------------------

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

void Core::AccountService::init(const Api &api, bool isTestnet)
{
    m_mediator->init(api, isTestnet);
}

void Core::AccountService::run()
{
    postTo(m_mediator.get(), [m = m_mediator.get()]() { m->runStreamer(); });
}

void Core::AccountService::restart()
{
    postTo(m_mediator.get(), [m = m_mediator.get()]() { m->restartStreamer(); });
}

void Core::AccountService::shutdown()
{
    QEventLoop loop;
    QTimer::singleShot(5000, &loop, &QEventLoop::quit);

    connect(m_mediator.get(), &Core::AccountMediator::streamerStopped,
            &loop, &QEventLoop::quit, Qt::SingleShotConnection);

    postTo(m_mediator.get(), [m = m_mediator.get()]() { m->stopStreamer(); });

    loop.exec();
}

void Core::AccountService::subscribe()
{
    postTo(m_mediator.get(), [m = m_mediator.get()]() { m->subscribe(); });
}

void Core::AccountService::requestWalletBalance()
{
    postTo(m_mediator.get(), [m = m_mediator.get()]() { m->requestWalletBalance(); });
}

void Core::AccountService::requestInfoAboutApi()
{
    postTo(m_mediator.get(), [m = m_mediator.get()]() { m->requestInfoAboutApi(); });
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
    emit messageReceived(QLatin1String("Streamer (id : %1) started").arg(id));
    emit streamerStarted(id);
}

void Core::AccountService::onStreamerStopped(const QString& id)
{
    emit messageReceived(QLatin1String("Streamer (id : %1) stopped").arg(id));
    emit streamerStopped(id);
}

void Core::AccountService::createOrder(const QVariantMap& params)
{
    if (!m_enableTrades)
    {
        emit warningOccurred("[createOrder] Trading was disabled");
        return;
    }

    validateAndSend(params, Tools::buildOrderRequest, [this](const Tools::OrderRequest& req) {
        postTo(m_mediator.get(), [m = m_mediator.get(), req] { m->requestCreateOrder(req); });
    });
}

void Core::AccountService::amendOrder(const QVariantMap& params)
{
    if (!m_enableTrades)
    {
        emit warningOccurred("[amendOrder] Trading was disabled");
        return;
    }

    validateAndSend(params, Tools::buildOrderAmendRequest, [this](const Tools::OrderAmendRequest& req) {
        postTo(m_mediator.get(), [m = m_mediator.get(), req] { m->requestAmendOrder(req); });
    });
}

void Core::AccountService::cancelOrder(const QVariantMap& params)
{
    if (!m_enableTrades)
    {
        emit warningOccurred("[cancelOrder] Trading was disabled");
        return;
    }

    validateAndSend(params, Tools::buildOrderCancelRequest, [this](const Tools::OrderCancelRequest& req) {
        postTo(m_mediator.get(), [m = m_mediator.get(), req] { m->requestCancelOrder(req); });
    });
}

void Core::AccountService::cancelAllOrders(const QVariantMap& params)
{
    if (!m_enableTrades)
    {
        emit warningOccurred("[cancelAllOrders] Trading was disabled");
        return;
    }

    validateAndSend(params, Tools::buildOrderCancelAllRequest, [this](const Tools::OrderCancelAllRequest& req) {
        postTo(m_mediator.get(), [m = m_mediator.get(), req] { m->requestCancelAllOrders(req); });
    });
}

void Core::AccountService::requestOpenOrders(const QVariantMap& params)
{
    validateAndSend(params, Tools::buildOpenOrdersRequest, [this](const Tools::OpenOrdersRequest& req) {
        postTo(m_mediator.get(), [m = m_mediator.get(), req] { m->requestOpenOrders(req); });
    });
}

void Core::AccountService::requestOrderHistory(const QVariantMap& params)
{
    validateAndSend(params, Tools::buildOrderHistoryRequest, [this](const Tools::OrderHistoryRequest& req) {
        postTo(m_mediator.get(), [m = m_mediator.get(), req] { m->requestOrderHistory(req); });
    });
}

void Core::AccountService::requestTradeHistory(const QVariantMap &params)
{
    validateAndSend(params, Tools::buildTradeHistoryRequest, [this](const Tools::TradeHistoryRequest& req) {
        postTo(m_mediator.get(), [m = m_mediator.get(), req] { m->requestTradeHistory(req); });
    });
}

void Core::AccountService::requestFeeRates(const QVariantMap &params)
{
    validateAndSend(params, Tools::buildFeeRatesRequest, [this](const Tools::FeeRateRequest& req) {
        postTo(m_mediator.get(), [m = m_mediator.get(), req] { m->requestFeeRates(req); });
    });
}

void Core::AccountService::exportOrders(const QString &path, Tools::Exporter::ExportFormat format, const QVariantMap &opts, const QVariantMap &params)
{
    auto req = Tools::buildStdExport(params);
    if (!req.has_value())
    {
        emit errorOccurred(req.error());
        return;
    }

    const QUrl url(path);
    const QString localPath = url.isLocalFile() ? url.toLocalFile() : path;

    postTo(m_mediator.get(), [=, this, self = QPointer(this), m = m_mediator.get()] {
        auto orders = m_mediator->readOrdersFromRepository(req.value());

        QMetaObject::invokeMethod(self, [=, this] {
            if (orders.isEmpty())
            {
                emit warningOccurred("[export] no orders");
                return;
            }

            doExport(orders, format, localPath, [](const OrderInfo& o) { return Tools::Exporter::toField(o); }, opts);
        });
    });

}

void Core::AccountService::exportExecs(const QString &path, Tools::Exporter::ExportFormat format, const QVariantMap &opts, const QVariantMap &params)
{
    auto req = Tools::buildStdExport(params);
    if (!req.has_value())
    {
        emit errorOccurred(req.error());
        return;
    }

    const QUrl url(path);
    const QString localPath = url.isLocalFile() ? url.toLocalFile() : path;

    postTo(m_mediator.get(), [=, this, self = QPointer(this), m = m_mediator.get()] {
        auto execs = m_mediator->readExecsFromRepository(req.value());

        QMetaObject::invokeMethod(self, [=, this] {
            if (execs.isEmpty())
            {
                emit warningOccurred("[export] No execs");
                return;
            }

            doExport(execs, format, localPath, [](const ExecutionInfo& e) { return Tools::Exporter::toField(e); }, opts);
        });
    });

}
