#include "ExecutionRepository.hpp"
#include "SqlHelpers.hpp"

namespace {

using namespace Core::Tools;

const QString kTable = QStringLiteral("execs");

const QStringList kColumns = {
    "category", "symbol", "side", "exec_price", "exec_qty", "exec_value",
    "order_id", "order_link_id", "exec_id", "exec_type", "exec_fee",
    "fee_currency", "is_maker", "exec_time"
};

const QString kUpsertSql = Sql::upsert(
    kTable,
    kColumns,
    { "category", "exec_id" }
    );

const QString kSelectSql = Sql::selectFrom(
    kTable,
    kColumns,
    QStringLiteral("category = ? AND symbol = ?"),
    QStringLiteral("exec_time ASC")
    );

QVariantList structureBinding(const ExecutionInfo& e)
{
    return {
        EnumHelpers::marketTypeToApiString(e.m_category),
        e.m_symbol,
        EnumHelpers::orderSideToString(e.m_side),
        e.m_execPrice,
        e.m_execQty,
        e.m_execValue,
        e.m_orderId,
        e.m_orderLinkId,
        e.m_execId,
        EnumHelpers::execTypeToString(e.m_execType),
        e.m_execFee,
        e.m_feeCurrency,
        e.m_isMaker ? 1 : 0,
        e.m_execTime
    };
}

} // namespace


void Core::Tools::ExecutionRepository::handleQuery(QSqlQuery& query)
{
    while (query.next())
    {
        ExecutionInfo e;
        e.m_category    = EnumHelpers::apiStringToMarketType(query.value("category").toString());
        e.m_symbol      = query.value("symbol").toString();
        e.m_side        = EnumHelpers::stringToOrderSide(query.value("side").toString());
        e.m_execPrice   = query.value("exec_price").toString();
        e.m_execQty     = query.value("exec_qty").toString();
        e.m_execValue   = query.value("exec_value").toString();
        e.m_orderId     = query.value("order_id").toString();
        e.m_orderLinkId = query.value("order_link_id").toString();
        e.m_execId      = query.value("exec_id").toString();
        e.m_execType    = EnumHelpers::stringToExecType(query.value("exec_type").toString());
        e.m_execFee     = query.value("exec_fee").toString();
        e.m_feeCurrency = query.value("fee_currency").toString();
        e.m_isMaker     = query.value("is_maker").toBool();
        e.m_execTime    = query.value("exec_time").toLongLong();
        m_data.append(e);
    }
}

Core::Tools::ExecutionRepository::ExecutionRepository(const QString &dbPath, IDatabaseManager &manager)
    : BaseExecutionsRepository(dbPath, manager) {}

bool Core::Tools::ExecutionRepository::init()
{
    QStringList queries;

    queries << R"(
        CREATE TABLE IF NOT EXISTS execs (
            id             INTEGER PRIMARY KEY,
            category       TEXT NOT NULL,
            symbol         TEXT NOT NULL,
            side           TEXT NOT NULL,
            exec_price     TEXT NOT NULL,
            exec_qty       TEXT NOT NULL,
            exec_value     TEXT NOT NULL,
            order_id       TEXT NOT NULL,
            order_link_id  TEXT,
            exec_id        TEXT NOT NULL,
            exec_type      TEXT NOT NULL DEFAULT 'Trade',
            exec_fee       TEXT,
            fee_currency   TEXT,
            is_maker       INTEGER NOT NULL DEFAULT 0,
            exec_time      INTEGER NOT NULL,
            UNIQUE(category, exec_id)
        );
    )";

    queries << "CREATE INDEX IF NOT EXISTS idx_execs_symbol_time ON execs(category, symbol, exec_time);";
    queries << "CREATE INDEX IF NOT EXISTS idx_execs_order ON execs(order_id);";
    queries << "CREATE INDEX IF NOT EXISTS idx_execs_link ON execs(order_link_id);";

    return m_dbManager.executeTransaction(m_dbPath, queries);
}

bool Core::Tools::ExecutionRepository::clear()
{
    if (!m_dbManager.beginTransaction(m_dbPath))
        return false;

    if (!m_dbManager.executeQuery(m_dbPath, "DELETE FROM execs")) {
        m_dbManager.rollbackTransaction(m_dbPath);
        return false;
    }

    return m_dbManager.commitTransaction(m_dbPath);
}

bool Core::Tools::ExecutionRepository::insert(const Executions &newExecs)
{
    if (newExecs.isEmpty())
        return true;

    if (!m_dbManager.beginTransaction(m_dbPath))
        return false;

    for (const auto& item : newExecs)
    {
        if (!m_dbManager.executePrepared(m_dbPath, kUpsertSql, structureBinding(item)))
        {
            m_dbManager.rollbackTransaction(m_dbPath);
            return false;
        }
    }

    return m_dbManager.commitTransaction(m_dbPath);
}

bool Core::Tools::ExecutionRepository::insert(const ExecutionInfo &newExecs)
{
    return m_dbManager.executePrepared(m_dbPath, kUpsertSql, structureBinding(newExecs));
}

bool Core::Tools::ExecutionRepository::select(EnumHelpers::MarketType type, const QString &symbol)
{
    m_data.clear();
    return m_dbManager.executePrepared(
        m_dbPath,
        kSelectSql,
        {
            EnumHelpers::marketTypeToApiString(type),
            symbol
        },
        [this](QSqlQuery& query) { handleQuery(query); }
        );
}