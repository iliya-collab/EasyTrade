#include "MarketDataRepository.hpp"
#include "Tools/Database/SqliteDatabaseManager.hpp"
#include "Tools/Repository/RepositoryCreator.hpp"
#include "Tools/Repository/DatabasePathProvider.hpp"

namespace Core::Repository
{

MarketDataRepository::MarketDataRepository(QObject* parent) : QObject(parent) {}

void MarketDataRepository::init(bool testnet)
{
    const QString baseDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    DatabasePathProvider pathToDB(baseDir);
    auto path = pathToDB.pathFor({dbScope::Market, testnet, {}});

    if (!path.has_value())
        throw std::runtime_error(path.error().toStdString());

    const QString dbFile = path.value();
    auto& manager = SqliteDatabaseManager::instance();

    if (!manager.open(dbFile))
        throw std::invalid_argument(manager.error().toStdString());

    const auto& creator = RepositoryCreator::instance();

    try
    {
        m_klinesRep         = std::move(creator.create<KlinesRepository>(dbFile, manager));
        m_publicTradesRep   = std::move(creator.create<PublicTradesRepository>(dbFile, manager));
    }
    catch (const std::exception& e)
    {
        emit errorOccurred(e.what());
        throw;
    }
}

Klines MarketDataRepository::loadKlines(EnumHelpers::MarketType type,
                                                     const QString &symbol,
                                                     EnumHelpers::Interval interval,
                                                     std::optional<qint64> start,
                                                     std::optional<qint64> end)
{
    QMutexLocker locker(&m_mutex);
    bool valid = start.has_value() && end.has_value() && (*start < *end);
    bool success = !valid ? m_klinesRep->select(type, symbol, interval) :
                       m_klinesRep->select(type, symbol, interval, start.value(), end.value());
    if (!success)
    {
        emit errorOccurred(m_klinesRep->error());
        return {};
    }
    return m_klinesRep->getData();
}

PublicTrades MarketDataRepository::loadPublicTrades(EnumHelpers::MarketType type, const QString &symbol)
{
    QMutexLocker locker(&m_mutex);
    if (!m_publicTradesRep->select(type, symbol))
    {
        emit errorOccurred(m_publicTradesRep->error());
        return {};
    }
    return m_publicTradesRep->getData();
}

void MarketDataRepository::clearKlines()
{
    QMutexLocker locker(&m_mutex);
    if (!m_klinesRep->clear())
    {
        emit errorOccurred(m_klinesRep->error());
        return;
    }
}

void MarketDataRepository::clearPublicTrades()
{
    QMutexLocker locker(&m_mutex);
    if (!m_publicTradesRep->clear())
    {
        emit errorOccurred(m_publicTradesRep->error());
        return;
    }
}

void MarketDataRepository::saveKlines(const KlineInfo& newKline)
{
    QMutexLocker locker(&m_mutex);
    if (!m_klinesRep->insert(newKline))
        emit errorOccurred(m_klinesRep->error());
}

void MarketDataRepository::saveKlines(const Klines& newKlines)
{
    QMutexLocker locker(&m_mutex);
    if (!m_klinesRep->insert(newKlines))
        emit errorOccurred(m_klinesRep->error());
}

void MarketDataRepository::savePublicTrades(const PublicTrades newTrades)
{
    QMutexLocker locker(&m_mutex);
    if (!m_publicTradesRep->insert(newTrades))
        emit errorOccurred(m_publicTradesRep->error());
}


}

