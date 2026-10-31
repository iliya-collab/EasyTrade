#pragma once

#include "IDatabaseManager.hpp"
#include <QSqlDatabase>
#include <QSqlError>
#include <QHash>
#include <QSet>
#include <QMutex>
#include <memory>

namespace Core::Tools
{

/*
 * Потокобезопасный менеджер SQLite/SQLCipher.
 */
class SqliteDatabaseManager : public IDatabaseManager
{
public:

    static SqliteDatabaseManager& instance();

    // Управление БД
    bool open(const QString& dbPath,
              DatabaseSqlType type = DatabaseSqlType::QSQLITE,
              const QString& password = QString()) override;
    void close(const QString& dbPath) override;
    bool closeAll() override;
    bool isOpen(const QString& dbPath) const override;
    QStringList openedConnections() const override;
    QString error() const override;
    bool existDBFile(const QString& name) const override;

    // Выполнение запросов для конкретной БД
    bool executeQuery(const QString& dbPath, const QString& query, const std::function<void(QSqlQuery&)>& callback = nullptr) override;
    bool executePrepared(const QString& dbPath, const QString& query, const QVariantList& values, const std::function<void(QSqlQuery&)>& callback = nullptr) override;

    // Транзакции для конкретной БД
    bool beginTransaction(const QString& dbPath) override;
    bool commitTransaction(const QString& dbPath) override;
    bool rollbackTransaction(const QString& dbPath) override;
    bool executeTransaction(const QString& dbPath, const QStringList& queries) override;
    bool rollbackAllTransactions() override;

private:

    struct DatabaseInfo
    {
        QString dbPath;
        QString connectionName;
        quintptr threadId = 0;
        bool inTransaction = false;
        int transactionDepth = 0;
    };
    using InfoPtr = std::shared_ptr<DatabaseInfo>;

    SqliteDatabaseManager() = default;

    SqliteDatabaseManager(const SqliteDatabaseManager&) = delete;
    SqliteDatabaseManager& operator=(const SqliteDatabaseManager&) = delete;

    QString createConnectionName(const QString& dbPath) const;
    static quintptr currentThreadId();

    InfoPtr findInfo(const QString& dbPath) const;
    bool acquire(const QString& dbPath, InfoPtr& info, QSqlDatabase& db) const;

    void closeForThread(quintptr threadId);
    void watchCurrentThread();

    QHash<QString, InfoPtr> m_connections;   // ключ - имя соединения
    QSet<quintptr> m_watchedThreads;
    mutable QMutex m_mutex;
};

}