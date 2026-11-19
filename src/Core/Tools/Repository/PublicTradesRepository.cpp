#include "PublicTradesRepository.hpp"

namespace {

constexpr auto kTradesTable = "trades";

constexpr auto kColumns =
    "category, symbol, side, price, volume, time";

const QString kSelectTradesSql = QStringLiteral(
                                     "SELECT %1 FROM %2 "
                                     "WHERE category = ? AND symbol = ? "
                                     "ORDER BY time ASC"
                                     ).arg(kColumns, kTradesTable);

const QString kInsertTradeSql = QStringLiteral(R"(
    INSERT INTO %1 (%2)
    VALUES (?, ?, ?, ?, ?, ?)
)").arg(kTradesTable, kColumns);

QVariantList tradeBindings(const PublicTradeInfo& t)
{
    return {
        EnumHelpers::marketTypeToApiString(t.m_category),
        t.m_symbol,
        EnumHelpers::orderSideToString(t.m_side),
        t.m_price,
        t.m_volume,
        t.m_tradeTime
    };
}

} // namespace

namespace Core::Tools
{

void PublicTradesRepository::handleSelectedPublicTrades(QSqlQuery& query)
{
    while (query.next())
    {
        PublicTradeInfo t;
        t.m_category  = EnumHelpers::apiStringToMarketType(query.value("category").toString());
        t.m_symbol    = query.value("symbol").toString();
        t.m_side      = EnumHelpers::stringToOrderSide(query.value("side").toString());
        t.m_price     = query.value("price").toString();
        t.m_volume    = query.value("volume").toString();
        t.m_tradeTime = query.value("time").toLongLong();
        t.m_turnover  = QString::number(t.m_price.toDouble() * t.m_volume.toDouble());
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
    return m_dbManager.executeQuery(m_dbPath, "DELETE FROM trades");
}

bool PublicTradesRepository::insert(const PublicTrades& newPublicTrades)
{
    if (newPublicTrades.isEmpty())
        return true;

    if (!m_dbManager.beginTransaction(m_dbPath))
        return false;

    for (const auto& item : newPublicTrades)
    {
        if (!m_dbManager.executePrepared(m_dbPath, kInsertTradeSql, tradeBindings(item)))
        {
            m_dbManager.rollbackTransaction(m_dbPath);
            return false;
        }
    }

    return m_dbManager.commitTransaction(m_dbPath);
}

bool PublicTradesRepository::select(EnumHelpers::MarketType type, const QString& symbol)
{
    m_data.clear();
    return m_dbManager.executePrepared(
        m_dbPath,
        kSelectTradesSql,
        {
            EnumHelpers::marketTypeToApiString(type),
            symbol
        },
        [this](QSqlQuery& query) { handleSelectedPublicTrades(query); }
        );
}

}