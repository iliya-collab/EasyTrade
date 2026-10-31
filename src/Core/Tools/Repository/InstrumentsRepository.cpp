#include "InstrumentsRepository.hpp"
#include "SqlHelpers.hpp"

namespace {

using namespace Core::Tools;

const QString kTable = QStringLiteral("instruments");

const QStringList kColumns = {
    "category", "symbol", "base_coin", "quote_coin"
};

const QString kUpsertSql = Sql::upsert(
    kTable,
    kColumns,
    { "category", "symbol" }
);

const QString kSelectSql = Sql::selectFrom(
    kTable,
    kColumns,
    QStringLiteral("category = ?")
);


QVariantList instrumentBindings(const InstrumentInfo& i)
{
    const QVariantList v = {
        EnumHelpers::marketTypeToApiString(i.m_category),
        i.m_symbol,
        i.m_baseCoin,
        i.m_quoteCoin
    };
    return v;
}

} // namespace

void Core::Tools::InstrumentsRepository::handleSelectedInstruments(QSqlQuery &query)
{
    while (query.next())
    {
        InstrumentInfo info;
        info.m_category = Tools::EnumHelpers::apiStringToMarketType(query.value("category").toString());
        info.m_symbol = query.value("symbol").toString();
        info.m_baseCoin = query.value("base_coin").toString();
        info.m_quoteCoin = query.value("quote_coin").toString();
        m_data.push_back(info);
    }
}

Core::Tools::InstrumentsRepository::InstrumentsRepository(const QString &dbPath, IDatabaseManager &manager)
    : BaseRepository(dbPath, manager) {};

bool Core::Tools::InstrumentsRepository::init()
{
    QStringList queries;

    queries << R"(
        CREATE TABLE IF NOT EXISTS instruments (
            id INTEGER PRIMARY KEY,
            category TEXT NOT NULL,
            symbol TEXT NOT NULL,
            base_coin TEXT,
            quote_coin TEXT,
            UNIQUE(category, symbol)
        )
    )";

    queries << "CREATE INDEX IF NOT EXISTS idx_instruments_quote_coin ON instruments(quote_coin)";

    return m_dbManager.executeTransaction(m_dbPath, queries);
}

bool Core::Tools::InstrumentsRepository::clear()
{
    if (!m_dbManager.beginTransaction(m_dbPath))
        return false;

    if (!m_dbManager.executeQuery(m_dbPath, QStringLiteral("DELETE FROM %1").arg(kTable))) {
        m_dbManager.rollbackTransaction(m_dbPath);
        return false;
    }

    return m_dbManager.commitTransaction(m_dbPath);
}

bool Core::Tools::InstrumentsRepository::insertInstruments(const QList<RowType> &newInstruments)
{
    if (newInstruments.isEmpty())
        return true;

    if (!m_dbManager.beginTransaction(m_dbPath))
        return false;

    for (const auto& item : newInstruments)
    {
        if (!m_dbManager.executePrepared(m_dbPath, kUpsertSql, instrumentBindings(item)))
        {
            m_dbManager.rollbackTransaction(m_dbPath);
            return false;
        }
    }

    return m_dbManager.commitTransaction(m_dbPath);
}

bool Core::Tools::InstrumentsRepository::selectInstruments(EnumHelpers::MarketType type)
{
    m_data.clear();
    return m_dbManager.executePrepared(
        m_dbPath,
        kSelectSql,
        {
            EnumHelpers::marketTypeToApiString(type)
        },
        [this](QSqlQuery& query) { handleSelectedInstruments(query); }
        );
}
