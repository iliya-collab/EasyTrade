#ifndef ACCOUNTREPOSITORY_HPP
#define ACCOUNTREPOSITORY_HPP
#include <QObject>
#include "Tools/Repository/OrdersRepository.hpp"
#include "Tools/Repository/ExecutionRepository.hpp"

namespace Core::Repository
{

class AccountDataRepository : public QObject
{
    Q_OBJECT
private:

    std::unique_ptr<Tools::OrdersRepository> m_ordersRep;
    std::unique_ptr<Tools::ExecutionRepository> m_execsRep;
    mutable QMutex m_mutex;

public:

    explicit AccountDataRepository(QObject* parent = nullptr);
    void init(const QString& userId, bool testnet);

    Tools::Orders loadOrders(Tools::EnumHelpers::MarketType type,
                            const QString& symbol);
    void clearOrders();
    void saveOrder(const Tools::OrderInfo& newOrder);
    void saveOrders(const Tools::Orders& newOrders);
    Tools::Orders getOrders() const { return m_ordersRep->getData(); }

    Tools::Executions loadExecs(Tools::EnumHelpers::MarketType type,
                                const QString& symbol);
    void clearExecs();
    void saveExec(const Tools::ExecutionInfo& newExec);
    void saveExecs(const Tools::Executions& newExecs);
    Tools::Executions getExecs() const { return m_execsRep->getData(); }

signals:

    void errorOccurred(const QString& error);
};

}

#endif // ACCOUNTREPOSITORY_HPP
