#include "KlinesRepository.hpp"

namespace Core::Tools
{

    void KlinesRepository::handleSelectedKlines(QSqlQuery &query)
    {
        while (query.next())
        {
            Kline k;
            k.m_category = Tools::stringToMarketType(query.value(0).toString());
            k.m_symbol   = query.value(1).toString();
            k.m_interval = Tools::stringToInterval(query.value(2).toString());
            k.m_start    = query.value(3).toLongLong();
            k.m_end      = query.value(4).toLongLong();
            k.m_open     = query.value(5).toDouble();
            k.m_close    = query.value(6).toDouble();
            k.m_high     = query.value(7).toDouble();
            k.m_low      = query.value(8).toDouble();
            k.m_volume   = query.value(9).toDouble();
            k.m_turnover = query.value(10).toDouble();
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

        queries << "CREATE INDEX IF NOT EXISTS idx_candles_category ON candles(category)";
        queries << "CREATE INDEX IF NOT EXISTS idx_candles_symbol ON candles(symbol)";
        queries << "CREATE INDEX IF NOT EXISTS idx_candles_interval ON candles(interval)";
        queries << "CREATE INDEX IF NOT EXISTS idx_candles_start ON candles(start)";
        queries << "CREATE INDEX IF NOT EXISTS idx_candles_end ON candles(end)";

        return m_dbManager.executeTransaction(m_dbPath, queries);
    }

    bool KlinesRepository::open() {
        return m_dbManager.open(m_dbPath);
    }

    void KlinesRepository::close() {
        m_dbManager.close(m_dbPath);
    }

    QString KlinesRepository::error() {
        return m_dbManager.error();
    }

    bool KlinesRepository::clear() {
        if (!m_dbManager.beginTransaction(m_dbPath))
            return false;

        if (!m_dbManager.executeQuery(m_dbPath, "DELETE FROM candles")) {
            m_dbManager.rollbackTransaction(m_dbPath);
            return false;
        }

        return m_dbManager.commitTransaction(m_dbPath);
    }

    bool KlinesRepository::insertKlines(const QList<Kline>& newKlines) {
        if (newKlines.isEmpty())
            return true;

        if (!m_dbManager.beginTransaction(m_dbPath))
            return false;

        for (const auto& item : newKlines)
            if (!m_dbManager.executePrepared(m_dbPath,
                R"(
                    INSERT INTO candles (category, symbol, interval, start, end, open, close, high, low, volume, turnover)
                    VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
                    ON CONFLICT(category, symbol, interval, start, end)
                    DO UPDATE SET
                        open = EXCLUDED.open,
                        close = EXCLUDED.close,
                        high = EXCLUDED.high,
                        low = EXCLUDED.low,
                        volume = EXCLUDED.volume,
                        turnover = EXCLUDED.turnover;
                )",
                { Tools::marketTypeToString(item.m_category), item.m_symbol,
                    Tools::intervalToString(item.m_interval), item.m_start,
                    item.m_end, item.m_open, item.m_close,
                    item.m_high, item.m_low, item.m_volume, item.m_turnover }))
            {
                m_dbManager.rollbackTransaction(m_dbPath);
                return false;
            }

        return m_dbManager.commitTransaction(m_dbPath);
    }

    bool KlinesRepository::insertKline(const Kline& newKline) {
        return m_dbManager.executePrepared(m_dbPath,
            R"(
                INSERT INTO candles (category, symbol, interval, start, end, open, close, high, low, volume, turnover)
                VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
                    ON CONFLICT(category, symbol, interval, start, end)
                    DO UPDATE SET
                        open = EXCLUDED.open,
                        close = EXCLUDED.close,
                        high = EXCLUDED.high,
                        low = EXCLUDED.low,
                        volume = EXCLUDED.volume,
                        turnover = EXCLUDED.turnover;
            )",
            { Tools::marketTypeToString(newKline.m_category), newKline.m_symbol,
                Tools::intervalToString(newKline.m_interval), newKline.m_start,
                newKline.m_end, newKline.m_open, newKline.m_close,
                newKline.m_high, newKline.m_low, newKline.m_volume, newKline.m_turnover });
    }

    bool KlinesRepository::selectKlines(MarketType type, const QString &symbol, Interval interval)
    {
        m_data.clear();
        return m_dbManager.executePrepared(m_dbPath,
            R"(
                SELECT category, symbol, interval, start, end, open, close, high, low, volume, turnover
                FROM candles
                WHERE category = ? AND symbol = ? AND interval = ?
                ORDER BY start ASC
            )",
            {Tools::marketTypeToString(type), symbol, Tools::intervalToString(interval)},
            [this](QSqlQuery& query) { handleSelectedKlines(query); });
    }

    bool KlinesRepository::selectKlines(MarketType type, const QString &symbol, Interval interval, qint64 start, qint64 end)
    {
        m_data.clear();
        return m_dbManager.executePrepared(m_dbPath,
            R"(
                SELECT category, symbol, interval, start, end, open, close, high, low, volume, turnover
                FROM candles
                WHERE category = ? AND symbol = ? AND interval = ?
                    AND start >= ? AND end <= ?
                ORDER BY start ASC
            )",
           { Tools::marketTypeToString(type), symbol, Tools::intervalToString(interval), start, end },
           [this](QSqlQuery& query) { handleSelectedKlines(query); });
    }
}
