#include "AccountState.hpp"
#include "Tools/Logging.hpp"

namespace Core {

    AccountState::AccountState(QObject *parent) : QObject{parent}
    {
        m_walletController = std::make_shared<Tools::WalletController>();
        m_apiController = std::make_shared<Tools::ApiController>();
        m_orders = std::make_shared<Tools::OrderModel>();
        m_executions = std::make_shared<Tools::ExecutionModel>();
        m_positions = std::make_shared<Tools::PositionModel>();
    }

    void AccountState::setAccountVerified()
    {
        if (m_accountVerified)
            return;
        m_accountVerified = true;
        emit accountVerifiedChanged();
    }

    void AccountState::setApi(const Api& api)
    {
        m_apiController->setApi(api);
    }

    void AccountState::setApiInfo(const ApiInfo &apiInfo)
    {
        m_apiController->setInfo(apiInfo);
    }

    void AccountState::setFeeRates(const FeeRatesInfo &rates)
    {
        m_feeRates.insert(InstrumentInfo::makeId(rates.m_category, rates.m_symbol), rates);
    }

    void AccountState::upsertWallet(const WalletInfo& wallet)
    {
        m_walletController->updateWallet(wallet);
    }

    void AccountState::updateOrder(const OrderInfo &order)
    {
        m_orders->upsertOrder(order);
        emit ordersChanged();
    }

    void AccountState::updateOrders(const Orders &orders)
    {
        m_orders->upsertOrders(orders);
        emit ordersChanged();
    }

    void AccountState::updateExecution(const ExecutionInfo &execution)
    {
        m_executions->addExecution(execution);
        emit executionsChanged();
    }

    void AccountState::updateExecutions(const Executions &executions)
    {
        m_executions->addExecutionBatch(executions);
        emit executionsChanged();
    }

    void AccountState::updatePosition(const PositionInfo &position)
    {
        m_positions->upsertPosition(position);
        emit positionsChanged();
    }

    std::optional<FeeRatesInfo> AccountState::feeFor(const InstrumentInfo &i) const
    {
        const QString id = i.id();
        if (!m_feeRates.contains(id))
            return std::nullopt;
        return m_feeRates.value(id);
    }

}

