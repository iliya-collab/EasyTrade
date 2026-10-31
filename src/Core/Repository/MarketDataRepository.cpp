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

    Core::Tools::DatabasePathProvider pathToDB(baseDir);
    auto path = pathToDB.pathFor({Tools::dbScope::Market, testnet, {}});

    if (!path.has_value())
        throw std::runtime_error(path.error().toStdString());

    const QString dbFile = path.value();
    auto& manager = Tools::SqliteDatabaseManager::instance();

    if (!manager.open(dbFile))
        throw std::invalid_argument(manager.error().toStdString());

    const auto& creator = Tools::RepositoryCreator::instance();

    try
    {
        m_instrumentsRep    = std::move(creator.create<Tools::InstrumentsRepository>(dbFile, manager));
        m_klinesRep         = std::move(creator.create<Tools::KlinesRepository>(dbFile, manager));
        m_publicTradesRep   = std::move(creator.create<Tools::PublicTradesRepository>(dbFile, manager));
    }
    catch (const std::exception& e)
    {
        emit errorOccurred(e.what());
        throw;
    }
}

Tools::Instruments MarketDataRepository::loadInstruments(Tools::EnumHelpers::MarketType type)
{
    QMutexLocker locker(&m_mutex);
    if (!m_instrumentsRep->selectInstruments(type))
    {
        emit errorOccurred(m_instrumentsRep->error());
        return {};
    }
    return m_instrumentsRep->getData();
}

QList<Tools::Kline> MarketDataRepository::loadKlines(Tools::EnumHelpers::MarketType type,
                                                     const QString &symbol,
                                                     Tools::EnumHelpers::Interval interval,
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

Tools::PublicTrades MarketDataRepository::loadPublicTrades(Tools::EnumHelpers::MarketType type, const QString &symbol)
{
    QMutexLocker locker(&m_mutex);
    if (!m_publicTradesRep->select(type, symbol))
    {
        emit errorOccurred(m_publicTradesRep->error());
        return {};
    }
    return m_publicTradesRep->getData();
}

void MarketDataRepository::clearInstruments()
{
    QMutexLocker locker(&m_mutex);
    if (!m_instrumentsRep->clear())
    {
        emit errorOccurred(m_instrumentsRep->error());
        return;
    }
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

void MarketDataRepository::saveInstruments(const Tools::Instruments &newInstruments)
{
    QMutexLocker locker(&m_mutex);
    if (!m_instrumentsRep->insertInstruments(newInstruments))
    {
        emit errorOccurred(m_instrumentsRep->error());
        return;
    }
}

void MarketDataRepository::saveKlines(const Tools::Kline& newKline)
{
    QMutexLocker locker(&m_mutex);
    if (!m_klinesRep->insert(newKline))
        emit errorOccurred(m_klinesRep->error());
}

void MarketDataRepository::saveKlines(const QList<Tools::Kline>& newKlines)
{
    QMutexLocker locker(&m_mutex);
    if (!m_klinesRep->insert(newKlines))
        emit errorOccurred(m_klinesRep->error());
}

void MarketDataRepository::savePublicTrades(const Tools::PublicTrades newTrades)
{
    QMutexLocker locker(&m_mutex);
    if (!m_publicTradesRep->insert(newTrades))
        emit errorOccurred(m_publicTradesRep->error());
}


}

