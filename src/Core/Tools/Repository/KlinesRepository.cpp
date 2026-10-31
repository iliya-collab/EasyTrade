#include "KlinesRepository.hpp"

namespace {

constexpr auto kCandlesTable = "candles";

constexpr auto kColumns =
    "category, symbol, interval, start, end, "
    "open, close, high, low, volume, turnover";

const QString kSelectKlinesSql = QStringLiteral(
    "SELECT %1 FROM %2 "
    "WHERE category = ? AND symbol = ? AND interval = ? "
    "ORDER BY start ASC"
).arg(kColumns, kCandlesTable);

const QString kSelectKlinesRangeSql = QStringLiteral(
    "SELECT %1 FROM %2 "
    "WHERE category = ? AND symbol = ? AND interval = ? "
    "  AND start >= ? AND end <= ? "
    "ORDER BY start ASC"
).arg(kColumns, kCandlesTable);

const QString kUpsertKlineSql = QStringLiteral(R"(
    INSERT INTO %1 (%2)
    VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    ON CONFLICT(category, symbol, interval, start, end)
    DO UPDATE SET
        open     = EXCLUDED.open,
        close    = EXCLUDED.close,
        high     = EXCLUDED.high,
        low      = EXCLUDED.low,
        volume   = EXCLUDED.volume,
        turnover = EXCLUDED.turnover;
)").arg(kCandlesTable, kColumns);

QVariantList klineBindings(const Core::Tools::Kline& k)
{
    using Core::Tools::EnumHelpers;
    using Core::Tools::EnumHelpers;
    return {
        EnumHelpers::marketTypeToApiString(k.m_category), k.m_symbol,
        EnumHelpers::intervalToString(k.m_interval),
        k.m_start, k.m_end,
        k.m_open, k.m_close, k.m_high, k.m_low,
        k.m_volume, k.m_turnover
    };
}

} // namespace

namespace Core::Tools
{

    void KlinesRepository::handleSelectedKlines(QSqlQuery& query)
    {
        while (query.next())
        {
            Kline k;
            k.m_category = Tools::EnumHelpers::apiStringToMarketType(query.value("category").toString());
            k.m_symbol   = query.value("symbol").toString();
            k.m_interval = Tools::EnumHelpers::stringToInterval(query.value("interval").toString());
            k.m_start    = query.value("start").toLongLong();
            k.m_end      = query.value("end").toLongLong();
            k.m_open     = query.value("open").toString();
            k.m_close    = query.value("close").toString();
            k.m_high     = query.value("high").toString();
            k.m_low      = query.value("low").toString();
            k.m_volume   = query.value("volume").toString();
            k.m_turnover = query.value("turnover").toString();
            m_data.append(k);
        }
    }

    KlinesRepository::KlinesRepository(const QString& dbPath, IDatabaseManager& manager) :
        BaseRepository(dbPath, manager) {}

    bool KlinesRepository::init()
    {
        QStringList queries;

        queries << R"(
            CREATE TABLE IF NOT EXISTS candles (
                id INTEGER PRIMARY KEY,
                category TEXT NOT NULL,
                symbol TEXT NOT NULL,
                interval TEXT NOT NULL,
                start INTEGER NOT NULL,
                end INTEGER NOT NULL,
                open REAL,
                close REAL,
                high REAL,
                low REAL,
                volume REAL,
                turnover REAL,
                UNIQUE(category, symbol, interval, start, end)
            )
        )";

        return m_dbManager.executeTransaction(m_dbPath, queries);
    }

    bool KlinesRepository::clear()
    {
        return m_dbManager.executeQuery(m_dbPath, "DELETE FROM candles");
    }

    bool KlinesRepository::insert(const QList<Kline>& newKlines)
    {
        if (newKlines.isEmpty())
            return true;

        if (!m_dbManager.beginTransaction(m_dbPath))
            return false;

        for (const auto& item : newKlines)
        {
            if (!m_dbManager.executePrepared(m_dbPath, kUpsertKlineSql, klineBindings(item)))
            {
                m_dbManager.rollbackTransaction(m_dbPath);
                return false;
            }
        }

        return m_dbManager.commitTransaction(m_dbPath);
    }

    bool KlinesRepository::insert(const Kline& newKline)
    {
        return m_dbManager.executePrepared(m_dbPath, kUpsertKlineSql, klineBindings(newKline));
    }

    bool KlinesRepository::select(EnumHelpers::MarketType type, const QString &symbol, EnumHelpers::Interval interval)
    {
        m_data.clear();
        return m_dbManager.executePrepared(
            m_dbPath,
            kSelectKlinesSql,
            {
                EnumHelpers::marketTypeToApiString(type),
                symbol,
                EnumHelpers::intervalToString(interval)
            },
            [this](QSqlQuery& query) { handleSelectedKlines(query); }
        );
    }

    bool KlinesRepository::select(EnumHelpers::MarketType type, const QString &symbol, EnumHelpers::Interval interval, qint64 start, qint64 end)
    {
        m_data.clear();
        return m_dbManager.executePrepared(
            m_dbPath,
            kSelectKlinesRangeSql,
            {
                EnumHelpers::marketTypeToApiString(type),
                symbol,
                EnumHelpers::intervalToString(interval),
                start,
                end
            },
            [this](QSqlQuery& query) { handleSelectedKlines(query); }
        );
    }
}
