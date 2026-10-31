#include "Tools/Database/SqliteDatabaseManager.hpp"
#include "Tools/Repository/RepositoryCreator.hpp"
#include "Tools/Repository/DatabasePathProvider.hpp"
#include "AccountDataRepository.hpp"

Core::Repository::AccountDataRepository::AccountDataRepository(QObject *parent)
    : QObject(parent) {}


void Core::Repository::AccountDataRepository::init(const QString &userId, bool testnet)
{
    const QString baseDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    Core::Tools::DatabasePathProvider pathToDB(baseDir);
    auto path = pathToDB.pathFor({Tools::dbScope::Account, testnet, userId});

    if (!path.has_value())
        throw std::runtime_error(path.error().toStdString());

    const QString dbFile = path.value();
    auto& manager = Tools::SqliteDatabaseManager::instance();

    if (!manager.open(dbFile, Tools::SqliteDatabaseManager::DatabaseSqlType::SQLCIPHER, userId))
        throw std::invalid_argument(manager.error().toStdString());

    const auto& creator = Tools::RepositoryCreator::instance();

    try
    {
        m_ordersRep = std::move(creator.create<Tools::OrdersRepository>(dbFile, manager));
        m_execsRep = std::move(creator.create<Tools::ExecutionRepository>(dbFile, manager));
    }
    catch (const std::exception& e)
    {
        emit errorOccurred(e.what());
        throw;
    }
}

void Core::Repository::AccountDataRepository::clearOrders()
{
    QMutexLocker locker(&m_mutex);
    if (!m_ordersRep->clear())
    {
        emit errorOccurred(m_ordersRep->error());
        return;
    }
}

void Core::Repository::AccountDataRepository::saveOrder(const Tools::OrderInfo &newOrder)
{
    QMutexLocker locker(&m_mutex);
    if (!m_ordersRep->insert(newOrder))
    {
        emit errorOccurred(m_ordersRep->error());
        return;
    }
}

void Core::Repository::AccountDataRepository::saveOrders(const Tools::Orders &newOrders)
{
    QMutexLocker locker(&m_mutex);
    if (!m_ordersRep->insert(newOrders))
    {
        emit errorOccurred(m_ordersRep->error());
        return;
    }
}

void Core::Repository::AccountDataRepository::clearExecs()
{
    QMutexLocker locker(&m_mutex);
    if (!m_ordersRep->clear())
    {
        emit errorOccurred(m_ordersRep->error());
        return;
    }
}

void Core::Repository::AccountDataRepository::saveExec(const Tools::ExecutionInfo &newExec)
{
    QMutexLocker locker(&m_mutex);
    if (!m_execsRep->insert(newExec))
    {
        emit errorOccurred(m_execsRep->error());
        return;
    }
}

void Core::Repository::AccountDataRepository::saveExecs(const Tools::Executions &newExecs)
{
    QMutexLocker locker(&m_mutex);
    if (!m_execsRep->insert(newExecs))
    {
        emit errorOccurred(m_execsRep->error());
        return;
    }
}

Core::Tools::Executions Core::Repository::AccountDataRepository::loadExecs(Tools::EnumHelpers::MarketType type, const QString &symbol)
{
    QMutexLocker locker(&m_mutex);
    if (!m_execsRep->select(type, symbol))
    {
        emit errorOccurred(m_execsRep->error());
        return {};
    }
    return m_execsRep->getData();
}

QList<Core::Tools::OrderInfo> Core::Repository::AccountDataRepository::loadOrders(Tools::EnumHelpers::MarketType type, const QString &symbol)
{
    QMutexLocker locker(&m_mutex);
    if (!m_ordersRep->select(type, symbol))
    {
        emit errorOccurred(m_ordersRep->error());
        return {};
    }
    return m_ordersRep->getData();
}
