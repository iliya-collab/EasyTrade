#include "OrdersRepository.hpp"
#include "SqlHelpers.hpp"

namespace {

using namespace Core::Tools;

const QString kTable = QStringLiteral("orders");

const QStringList kColumns = {
    "category", "order_id", "order_link_id", "symbol", "side", "order_type", "status",
    "position_idx", "leaves_qty", "price", "qty", "cum_exec_qty", "cum_exec_value",
    "cum_exec_fee", "reject_reason", "created_time", "updated_time"
};

const QString kUpsertSql = Sql::upsert(
    kTable,
    kColumns,
    { "category", "order_id" },
    QStringLiteral("excluded.updated_time >= %1.updated_time").arg(kTable)
);

const QString kSelectSql = Sql::selectFrom(
    kTable,
    kColumns,
    QStringLiteral("category = ? AND symbol = ?"),
    QStringLiteral("created_time ASC")
);

QVariantList orderBindings(const OrderInfo& o)
{
    const QVariantList v = {
        EnumHelpers::marketTypeToApiString(o.m_category),
        o.m_orderId,
        o.m_orderLinkId,
        o.m_symbol,
        EnumHelpers::orderSideToString(o.m_side),
        EnumHelpers::orderTypeToString(o.m_orderType),
        EnumHelpers::orderStatusToString(o.m_status),
        o.m_positionIdx,
        o.m_leavesQty,
        o.m_price,
        o.m_qty,
        o.m_cumExecQty,
        o.m_cumExecValue,
        o.m_cumExecFee,
        o.m_rejectReason,
        o.m_createdTime,
        o.m_updatedTime
    };
    return v;
}

} // namespace


void Core::Tools::OrdersRepository::handleSelectedOrders(QSqlQuery& query)
{
    while (query.next())
    {
        OrderInfo o;
        o.m_category     = EnumHelpers::apiStringToMarketType(query.value("category").toString());
        o.m_orderId      = query.value("order_id").toString();
        o.m_orderLinkId  = query.value("order_link_id").toString();
        o.m_symbol       = query.value("symbol").toString();
        o.m_side         = EnumHelpers::stringToOrderSide(query.value("side").toString());
        o.m_orderType    = EnumHelpers::stringToOrderType(query.value("order_type").toString());
        o.m_status       = EnumHelpers::stringToOrderStatus(query.value("status").toString());
        o.m_positionIdx  = query.value("position_idx").toInt();
        o.m_leavesQty    = query.value("leaves_qty").toString();
        o.m_price        = query.value("price").toString();
        o.m_qty          = query.value("qty").toString();
        o.m_cumExecQty   = query.value("cum_exec_qty").toString();
        o.m_cumExecValue = query.value("cum_exec_value").toString();
        o.m_cumExecFee   = query.value("cum_exec_fee").toString();
        o.m_rejectReason = query.value("reject_reason").toString();
        o.m_createdTime  = query.value("created_time").toLongLong();
        o.m_updatedTime  = query.value("updated_time").toLongLong();
        m_data.append(o);
    }
}

Core::Tools::OrdersRepository::OrdersRepository(const QString &dbPath, IDatabaseManager &manager)
    : BaseOrdersRepository(dbPath, manager) {}

bool Core::Tools::OrdersRepository::init()
{
    QStringList queries;

    queries << R"(
        CREATE TABLE IF NOT EXISTS orders (
            id             INTEGER PRIMARY KEY,
            category       TEXT NOT NULL,
            order_id       TEXT NOT NULL,
            order_link_id  TEXT,
            symbol         TEXT NOT NULL,
            side           TEXT NOT NULL,
            order_type     TEXT NOT NULL,
            status         TEXT NOT NULL,
            position_idx   INTEGER NOT NULL DEFAULT 0,
            leaves_qty     TEXT,
            price          TEXT,
            qty            TEXT NOT NULL,
            cum_exec_qty   TEXT,
            cum_exec_value TEXT,
            cum_exec_fee   TEXT,
            reject_reason  TEXT,
            created_time   INTEGER NOT NULL,
            updated_time   INTEGER NOT NULL,
            UNIQUE(category, order_id)
        );
    )";

    queries << "CREATE INDEX IF NOT EXISTS idx_orders_symbol_time ON orders(category, symbol, created_time);";
    queries << "CREATE INDEX IF NOT EXISTS idx_orders_link ON orders(order_link_id);";

    return m_dbManager.executeTransaction(m_dbPath, queries);
}

bool Core::Tools::OrdersRepository::clear()
{
    if (!m_dbManager.beginTransaction(m_dbPath))
        return false;

    if (!m_dbManager.executeQuery(m_dbPath, "DELETE FROM orders")) {
        m_dbManager.rollbackTransaction(m_dbPath);
        return false;
    }

    return m_dbManager.commitTransaction(m_dbPath);
}

bool Core::Tools::OrdersRepository::insert(const QList<OrderInfo> &newOrders)
{
    if (newOrders.isEmpty())
        return true;

    if (!m_dbManager.beginTransaction(m_dbPath))
        return false;

    for (const auto& item : newOrders)
    {
        if (!m_dbManager.executePrepared(m_dbPath, kUpsertSql, orderBindings(item)))
        {
            m_dbManager.rollbackTransaction(m_dbPath);
            return false;
        }
    }

    return m_dbManager.commitTransaction(m_dbPath);
}

bool Core::Tools::OrdersRepository::insert(const OrderInfo &newOrder)
{
    return m_dbManager.executePrepared(m_dbPath, kUpsertSql, orderBindings(newOrder));
}

bool Core::Tools::OrdersRepository::select(EnumHelpers::MarketType type, const QString &symbol)
{
    m_data.clear();
    return m_dbManager.executePrepared(
        m_dbPath,
        kSelectSql,
        {
            EnumHelpers::marketTypeToApiString(type),
            symbol
        },
        [this](QSqlQuery& query) { handleSelectedOrders(query); }
    );
}
