#include "PublicTradesRepository.hpp"

namespace Core::Tools {

    PublicTradesRepository::PublicTradesRepository(const QString& dbPath, IDatabaseManager& manager) :
        BaseRepository(dbPath, manager) {}

    bool PublicTradesRepository::init() {

        QString query = R"(
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

        return m_dbManager.executeQuery(m_dbPath, query);
    }

    bool PublicTradesRepository::open() {
        return m_dbManager.open(m_dbPath);
    }

    void PublicTradesRepository::close() {
        m_dbManager.close(m_dbPath);
    }

    QString PublicTradesRepository::error() {
        return m_dbManager.error();
    }

    bool PublicTradesRepository::clear() {
        if (!m_dbManager.beginTransaction(m_dbPath))
            return false;

        if (!m_dbManager.executeQuery(m_dbPath, "DELETE FROM trades")) {
            m_dbManager.rollbackTransaction(m_dbPath);
            return false;
        }

        // if (!m_dbManager.executePrepared(m_dbPath, "UPDATE sqlite_sequence SET seq = 0 WHERE name = ?", {"trades"})) {
        //     m_dbManager.rollbackTransaction(m_dbPath);
        //     return false;
        // }

        return m_dbManager.commitTransaction(m_dbPath);
    }

}
