#include "PublicTradesRepository.hpp"

namespace Core::Tools {

void PublicTradesRepository::handleSelectedPublicTrades(QSqlQuery &query)
{
    while (query.next())
    {
        Tools::PublicTradeItem t;
        t.m_category        = Tools::stringToMarketType(query.value(0).toString());
        t.m_symbol          = query.value(1).toString();
        t.m_side            = Tools::stringToOrderSide(query.value(2).toString());
        t.m_price           = query.value(3).toString();
        t.m_volume          = query.value(4).toString();
        t.m_tradeTime       = query.value(5).toLongLong();
        t.m_turnover        = QString::number(t.m_price.toDouble() * t.m_volume.toDouble());
        m_data.append(t);
    }
}

PublicTradesRepository::PublicTradesRepository(const QString& dbPath, IDatabaseManager& manager) :
    BaseRepository(dbPath, manager) {}

bool PublicTradesRepository::init()
{

    QStringList queries;

    queries << R"(
        CREATE TABLE IF NOT EXISTS trades (
            id INTEGER PRIMARY KEY,
            category TEXT NOT NULL,
            symbol TEXT NOT NULL,
            side TEXT NOT NULL,
            price REAL,
            volume REAL,
            time INTEGER
        )
    )";

    queries << "CREATE INDEX IF NOT EXISTS idx_trades_category ON trades(category)";
    queries << "CREATE INDEX IF NOT EXISTS idx_trades_symbol ON trades(symbol)";

    return m_dbManager.executeTransaction(m_dbPath, queries);
}

bool PublicTradesRepository::open()
{
    return m_dbManager.open(m_dbPath);
}

void PublicTradesRepository::close()
{
    m_dbManager.close(m_dbPath);
}

QString PublicTradesRepository::error()
{
    return m_dbManager.error();
}

bool PublicTradesRepository::clear()
{
    if (!m_dbManager.beginTransaction(m_dbPath))
        return false;

    if (!m_dbManager.executeQuery(m_dbPath, "DELETE FROM trades"))
    {
        m_dbManager.rollbackTransaction(m_dbPath);
        return false;
    }

    return m_dbManager.commitTransaction(m_dbPath);
}

bool PublicTradesRepository::insertPublicTrades(const PublicTrades &newPublicTrades)
{
    if (newPublicTrades.isEmpty())
        return true;

    if (!m_dbManager.beginTransaction(m_dbPath))
        return false;

    for (const auto& item : newPublicTrades)
        if (!m_dbManager.executePrepared(m_dbPath,
            R"(
                INSERT INTO trades (category, symbol, side, price, volume, time)
                VALUES (?, ?, ?, ?, ?, ?)
            )",
            { Tools::marketTypeToString(item.m_category), item.m_symbol,
                Tools::orderSideToString(item.m_side), item.m_price,
                item.m_volume, item.m_tradeTime }))
        {
            m_dbManager.rollbackTransaction(m_dbPath);
            return false;
        }

    return m_dbManager.commitTransaction(m_dbPath);
}

bool PublicTradesRepository::selectKlines(MarketType type, const QString &symbol)
{
    m_data.clear();
    return m_dbManager.executePrepared(m_dbPath,
        R"(
            SELECT category, symbol, side, price, volume, time
            FROM trades
            WHERE category = ? AND symbol = ?
            ORDER BY time ASC
        )",
       { Tools::marketTypeToString(type), symbol },
       [this](QSqlQuery& query) { handleSelectedPublicTrades(query); });
}

}
