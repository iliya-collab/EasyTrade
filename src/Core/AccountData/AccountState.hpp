#pragma once
#include "Tools/DataModels/WalletController.hpp"
#include "Tools/DataModels/ApiController.hpp"
#include "Tools/DataModels/OrderModel.hpp"
#include "Tools/DataModels/ExecutionModel.hpp"
#include "Tools/DataModels/PositionModel.hpp"
#include <QObject>

namespace Core {

class AccountState : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool                             accountVerified         READ accountVerified            NOTIFY accountVerifiedChanged       FINAL)
    Q_PROPERTY(Tools::WalletController*         walletController        READ getWalletController        NOTIFY walletControllerChanged      FINAL)
    Q_PROPERTY(Tools::ApiController*            apiController           READ getApiController           NOTIFY apiControllerChanged         FINAL)
    Q_PROPERTY(Tools::OrderModel*               orders                  READ getOrders                  NOTIFY ordersChanged                FINAL)
    Q_PROPERTY(Tools::ExecutionModel*           executions              READ getExecutions              NOTIFY executionsChanged            FINAL)
    Q_PROPERTY(Tools::PositionModel*            positions               READ getPositions               NOTIFY positionsChanged             FINAL)

private:

    // Даннные аккаунта/пользователя
    bool m_accountVerified = false;
    QHash<QString, FeeRatesInfo> m_feeRates;
    std::shared_ptr<Tools::ApiController> m_apiController{};
    std::shared_ptr<Tools::WalletController> m_walletController{};
    std::shared_ptr<Tools::OrderModel> m_orders{};
    std::shared_ptr<Tools::ExecutionModel> m_executions{};
    std::shared_ptr<Tools::PositionModel> m_positions{};

public:

    explicit AccountState(QObject *parent = nullptr);

    // Методы обновления
    void setAccountVerified();
    void setApi(const Api& api);
    void setApiInfo(const ApiInfo& apiInfo);
    void setFeeRates(const FeeRatesInfo& rates);
    void upsertWallet(const WalletInfo& wallet);
    void updateOrder(const OrderInfo& order);
    void updateOrders(const QList<OrderInfo>& orders);
    void updateExecution(const ExecutionInfo& execution);
    void updateExecutions(const QList<ExecutionInfo>& executions);
    void updatePosition(const PositionInfo& position);

    // READ-методы
    bool accountVerified() const { return m_accountVerified; }
    Tools::WalletController* getWalletController() const { return m_walletController.get(); }
    Tools::ApiController* getApiController() const { return m_apiController.get(); }
    Tools::OrderModel* getOrders() const { return m_orders.get(); }
    Tools::ExecutionModel* getExecutions() const { return m_executions.get(); }
    Tools::PositionModel* getPositions() const { return m_positions.get(); }

    std::optional<FeeRatesInfo> feeFor(const InstrumentInfo& i) const;

signals:

    void accountVerifiedChanged();
    void walletControllerChanged();
    void apiControllerChanged();
    void ordersChanged();
    void executionsChanged();
    void positionsChanged();

};

}
