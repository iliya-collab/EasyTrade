#include "MarketDataRepository.hpp"
#include "Tools/Database/SqliteDatabaseManager.hpp"
#include "Tools/Repository/RepositoryCreator.hpp"

namespace Core::Markets
{

MarketDataRepository::MarketDataRepository(QObject* parent) : QObject(parent)
{
    qDebug() << Q_FUNC_INFO << "called from:" << QThread::currentThread();

    const QString dbFile = "db/crypto.db";

    auto& manager = Tools::SqliteDatabaseManager::instance();

    if (!manager.open(dbFile))
        throw std::invalid_argument(manager.error().toStdString());

    try
    {
        m_cryptoRep = std::move(Tools::RepositoryCreator::instance().create<Tools::CryptoRepository>(dbFile, manager));
        m_klinesRep = std::move(Tools::RepositoryCreator::instance().create<Tools::KlinesRepository>(dbFile, manager));
        m_publicTradesRep = std::move(Tools::RepositoryCreator::instance().create<Tools::PublicTradesRepository>(dbFile, manager));
    }
    catch (const std::exception& e)
    {
        qCritical() << e.what();
        throw;
    }
}

QList<Tools::TradeInfo> MarketDataRepository::loadFromCryptoRepository(Tools::MarketType type)
{
    qDebug() << Q_FUNC_INFO << "called from:" << QThread::currentThread();
    QMutexLocker locker(&m_mutex);
    if (!m_cryptoRep->selectTrades(type))
    {
        emit errorOccurred(m_cryptoRep->error());
        return QList<Tools::TradeInfo>();
    }
    return m_cryptoRep->getData();
}

QList<Tools::Kline> MarketDataRepository::loadFromKlinesRepository(Tools::MarketType type, const QString &symbol, Tools::Interval interval, std::optional<qint64> start, std::optional<qint64> end)
{
    qDebug() << Q_FUNC_INFO << "called from:" << QThread::currentThread();
    QMutexLocker locker(&m_mutex);
    bool valid = start.has_value() && end.has_value() && (*start > *end);
    bool success = !valid ? m_klinesRep->selectKlines(type, symbol, interval) :
                       m_klinesRep->selectKlines(type, symbol, interval, start.value(), end.value());
    if (!success)
    {
        emit errorOccurred(m_klinesRep->error());
        return {};
    }
    return m_klinesRep->getData();
}

Tools::PublicTrades MarketDataRepository::loadFromPublicTradesRepository(Tools::MarketType type, const QString &symbol)
{
    // TODO
    return {};
}

void MarketDataRepository::clearCryptoRepository() {
    qDebug() << Q_FUNC_INFO << "called from:" << QThread::currentThread();
    QMutexLocker locker(&m_mutex);
    if (!m_cryptoRep->clear())
    {
        emit errorOccurred(m_cryptoRep->error());
        return;
    }
}

void MarketDataRepository::clearKlinesRepository()
{
    qDebug() << Q_FUNC_INFO << "called from:" << QThread::currentThread();
    QMutexLocker locker(&m_mutex);
    if (!m_klinesRep->clear())
    {
        emit errorOccurred(m_klinesRep->error());
        return;
    }
}

void MarketDataRepository::clearPublicTradesRepository()
{
    qDebug() << Q_FUNC_INFO << "called from:" << QThread::currentThread();
    QMutexLocker locker(&m_mutex);
    if (!m_publicTradesRep->clear())
    {
        emit errorOccurred(m_publicTradesRep->error());
        return;
    }
}

void MarketDataRepository::saveToCryptoRepository(const QList<Tools::TradeInfo>& tradePairs) {
    qDebug() << Q_FUNC_INFO << "called from:" << QThread::currentThread();
    QMutexLocker locker(&m_mutex);
    if (!m_cryptoRep->insertTrades(tradePairs)) {
        emit errorOccurred(m_cryptoRep->error());
        return;
    }
}

void MarketDataRepository::saveToKlinesRepository(const Tools::Kline& newKline) {
    qDebug() << Q_FUNC_INFO << "called from:" << QThread::currentThread();
    QMutexLocker locker(&m_mutex);
    if (!m_klinesRep->insertKline(newKline))
        emit errorOccurred(m_klinesRep->error());
}

void MarketDataRepository::saveToKlinesRepository(const QList<Tools::Kline>& newKlines) {
    qDebug() << Q_FUNC_INFO << "called from:" << QThread::currentThread();
    QMutexLocker locker(&m_mutex);
    if (!m_klinesRep->insertKlines(newKlines))
        emit errorOccurred(m_klinesRep->error());
}

void MarketDataRepository::saveToPublicTradesRepository(const Tools::PublicTrades newTrades)
{

}


}

