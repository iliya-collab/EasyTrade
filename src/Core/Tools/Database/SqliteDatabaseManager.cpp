#include "SqliteDatabaseManager.hpp"
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QThread>
#include <QDebug>
#include <QDir>

namespace Core::Tools {

namespace {

thread_local QString t_lastError;

void setError(const QString& e)
{
    t_lastError = e;
}

void removeConnection(const QString& name)
{
    if (!QSqlDatabase::contains(name))
        return;

    {
        QSqlDatabase db = QSqlDatabase::database(name, false);
        if (db.isValid() && db.isOpen())
            db.close();
    }

    QSqlDatabase::removeDatabase(name);
}

bool runSimple(QSqlDatabase& db, const QString& sql)
{
    QSqlQuery q(db);
    return q.exec(sql);
}

bool openConnection(const QString& connName,
                    const QString& dbPath,
                    IDatabaseManager::DatabaseSqlType type,
                    const QString& password,
                    QString& err)
{
    const bool cipher = (type == IDatabaseManager::DatabaseSqlType::SQLCIPHER);

    QSqlDatabase db = QSqlDatabase::contains(connName)
                          ? QSqlDatabase::database(connName, false)
                          : QSqlDatabase::addDatabase(cipher ? QStringLiteral("SQLITECIPHER")
                                                             : QStringLiteral("QSQLITE"),
                                                      connName);

    if (db.isOpen())
        return true;

    db.setDatabaseName(dbPath);
    if (cipher && !password.isEmpty())
        db.setPassword(password);

    if (!db.open())
    {
        err = QStringLiteral("Error opening database %1: %2")
            .arg(dbPath, db.lastError().text());
        return false;
    }

    if (cipher)
    {
        QSqlQuery testQuery(db);
        if (!testQuery.exec(QStringLiteral("SELECT count(*) FROM sqlite_master;")))
        {
            err = QStringLiteral("SQLCipher authentication failed for %1: %2")
                .arg(dbPath, testQuery.lastError().text());
            return false;
        }
    }

    // Несколько соединений из разных потоков к одному файлу:
    // - busy_timeout: ждать освобождения блокировки вместо мгновенного "database is locked"
    // - WAL: читатели не блокируют писателя
    runSimple(db, QStringLiteral("PRAGMA busy_timeout=5000"));
    runSimple(db, QStringLiteral("PRAGMA journal_mode=WAL"));
    runSimple(db, QStringLiteral("PRAGMA synchronous=NORMAL"));

    return true;
}

} // namespace

SqliteDatabaseManager& SqliteDatabaseManager::instance()
{
    static SqliteDatabaseManager manager;
    return manager;
}

quintptr SqliteDatabaseManager::currentThreadId()
{
    return reinterpret_cast<quintptr>(QThread::currentThreadId());
}

QString SqliteDatabaseManager::error() const
{
    return t_lastError;
}

bool SqliteDatabaseManager::existDBFile(const QString& name) const
{
    return QFileInfo::exists(name);
}

QString SqliteDatabaseManager::createConnectionName(const QString& dbPath) const
{
    const QFileInfo fi(dbPath);
    const QString hash = QString::fromLatin1(
        QCryptographicHash::hash(fi.absoluteFilePath().toUtf8(), QCryptographicHash::Md5)
            .toHex().left(12));

    return QStringLiteral("%1_%2_conn_%3")
        .arg(fi.completeBaseName(), hash)
        .arg(currentThreadId());
}

SqliteDatabaseManager::InfoPtr SqliteDatabaseManager::findInfo(const QString& dbPath) const
{
    const QString connName = createConnectionName(dbPath);
    QMutexLocker locker(&m_mutex);
    return m_connections.value(connName);
}

bool SqliteDatabaseManager::acquire(const QString& dbPath, InfoPtr& info, QSqlDatabase& db) const
{
    info = findInfo(dbPath);
    if (!info)
    {
        setError(QStringLiteral("Database %1 is not opened").arg(dbPath));
        return false;
    }

    if (QSqlDatabase::contains(info->connectionName))
        db = QSqlDatabase::database(info->connectionName, false);

    if (!db.isValid() || !db.isOpen())
    {
        setError(QStringLiteral("Database %1 is not open").arg(dbPath));
        info->inTransaction = false;
        info->transactionDepth = 0;
        return false;
    }

    return true;
}

bool SqliteDatabaseManager::open(const QString& dbPath, DatabaseSqlType type, const QString& password)
{
    if (!existDBFile(dbPath))
    {
        QDir dir = QFileInfo(dbPath).absoluteDir();
        if (!dir.exists())
            dir.mkpath(".");
    }

    const QString connName = createConnectionName(dbPath);

    QString err;
    if (!openConnection(connName, dbPath, type, password, err))
    {
        removeConnection(connName);
        {
            QMutexLocker locker(&m_mutex);
            m_connections.remove(connName);
        }
        setError(err);
        return false;
    }

    {
        QMutexLocker locker(&m_mutex);
        InfoPtr info = m_connections.value(connName);
        if (!info)
        {
            info = std::make_shared<DatabaseInfo>();
            info->dbPath = dbPath;
            info->connectionName = connName;
            info->threadId = currentThreadId();
            m_connections.insert(connName, info);
        }
        else
        {
            info->inTransaction = false;
            info->transactionDepth = 0;
        }
    }

    watchCurrentThread();
    return true;
}

void SqliteDatabaseManager::watchCurrentThread()
{
    QThread* thread = QThread::currentThread();
    QCoreApplication* app = QCoreApplication::instance();

    if (!thread || !app || thread == app->thread())
        return;

    const quintptr tid = currentThreadId();

    {
        QMutexLocker locker(&m_mutex);
        if (m_watchedThreads.contains(tid))
            return;
        m_watchedThreads.insert(tid);
    }
    QObject::connect(thread, &QThread::finished, thread,
                     [this, tid]()
                     {
                         closeForThread(tid);
                         QMutexLocker locker(&m_mutex);
                         m_watchedThreads.remove(tid);
                     },
                     Qt::ConnectionType(Qt::DirectConnection | Qt::SingleShotConnection));
}

void SqliteDatabaseManager::closeForThread(quintptr threadId)
{
    QList<InfoPtr> mine;

    {
        QMutexLocker locker(&m_mutex);
        for (auto it = m_connections.begin(); it != m_connections.end(); )
        {
            if (it.value()->threadId == threadId)
            {
                mine.append(it.value());
                it = m_connections.erase(it);
            }
            else
                ++it;
        }
    }

    for (const InfoPtr& info : std::as_const(mine))
        removeConnection(info->connectionName);
}

void SqliteDatabaseManager::close(const QString& dbPath)
{
    const QString connName = createConnectionName(dbPath);

    InfoPtr info;
    {
        QMutexLocker locker(&m_mutex);
        info = m_connections.take(connName);
    }

    if (!info)
        return;

    removeConnection(info->connectionName);
}

bool SqliteDatabaseManager::closeAll()
{
    closeForThread(currentThreadId());
    return true;
}

bool SqliteDatabaseManager::isOpen(const QString& dbPath) const
{
    const InfoPtr info = findInfo(dbPath);
    if (!info || !QSqlDatabase::contains(info->connectionName))
        return false;

    const QSqlDatabase db = QSqlDatabase::database(info->connectionName, false);
    return db.isValid() && db.isOpen();
}

QStringList SqliteDatabaseManager::openedConnections() const
{
    QMutexLocker locker(&m_mutex);
    return m_connections.keys();
}

bool SqliteDatabaseManager::executeQuery(const QString& dbPath, const QString& query,
                                         const std::function<void(QSqlQuery&)>& callback)
{
    InfoPtr info;
    QSqlDatabase db;
    if (!acquire(dbPath, info, db))
        return false;

    QSqlQuery sqlQuery(db);

    if (!sqlQuery.exec(query))
    {
        setError(QStringLiteral("Query error on %1: %2").arg(dbPath, sqlQuery.lastError().text()));
        return false;
    }

    if (callback)
        callback(sqlQuery);

    return true;
}

bool SqliteDatabaseManager::executePrepared(const QString& dbPath, const QString& query, const QVariantList& values,
                                            const std::function<void(QSqlQuery&)>& callback)
{
    InfoPtr info;
    QSqlDatabase db;
    if (!acquire(dbPath, info, db))
        return false;

    QSqlQuery sqlQuery(db);

    if (!sqlQuery.prepare(query))
    {
        setError(QStringLiteral("Prepare error on %1: %2").arg(dbPath, sqlQuery.lastError().text()));
        return false;
    }

    for (const QVariant& value : values)
        sqlQuery.addBindValue(value);

    if (!sqlQuery.exec())
    {
        setError(QStringLiteral("Processing error on %1: %2").arg(dbPath, sqlQuery.lastError().text()));
        return false;
    }

    if (callback)
        callback(sqlQuery);

    return true;
}

bool SqliteDatabaseManager::beginTransaction(const QString& dbPath)
{
    InfoPtr info;
    QSqlDatabase db;
    if (!acquire(dbPath, info, db))
        return false;

    QSqlQuery query(db);

    if (info->inTransaction)
    {
        const QString savepointName = QStringLiteral("savepoint_%1").arg(info->transactionDepth);

        if (!query.exec(QStringLiteral("SAVEPOINT %1").arg(savepointName)))
        {
            setError(QStringLiteral("Failed to create savepoint: %1").arg(query.lastError().text()));
            return false;
        }

        info->transactionDepth++;
        return true;
    }

    if (!query.exec(QStringLiteral("BEGIN IMMEDIATE TRANSACTION")))
    {
        setError(QStringLiteral("Failed to begin transaction: %1").arg(query.lastError().text()));
        return false;
    }

    info->inTransaction = true;
    info->transactionDepth = 1;

    return true;
}

bool SqliteDatabaseManager::commitTransaction(const QString& dbPath)
{
    InfoPtr info;
    QSqlDatabase db;
    if (!acquire(dbPath, info, db))
        return false;

    if (!info->inTransaction)
    {
        setError(QStringLiteral("No active transaction to commit in database %1").arg(dbPath));
        return false;
    }

    QSqlQuery query(db);

    if (info->transactionDepth > 1)
    {
        const QString savepointName = QStringLiteral("savepoint_%1").arg(info->transactionDepth - 1);

        if (!query.exec(QStringLiteral("RELEASE SAVEPOINT %1").arg(savepointName)))
        {
            setError(QStringLiteral("Failed to release savepoint: %1").arg(query.lastError().text()));
            return false;
        }

        info->transactionDepth--;
        return true;
    }

    if (!query.exec(QStringLiteral("COMMIT")))
    {
        setError(QStringLiteral("Failed to commit transaction: %1").arg(query.lastError().text()));

        query.exec(QStringLiteral("ROLLBACK"));
        info->inTransaction = false;
        info->transactionDepth = 0;
        return false;
    }

    info->inTransaction = false;
    info->transactionDepth = 0;

    return true;
}

bool SqliteDatabaseManager::rollbackTransaction(const QString& dbPath)
{
    InfoPtr info;
    QSqlDatabase db;
    if (!acquire(dbPath, info, db))
        return false;

    if (!info->inTransaction)
    {
        setError(QStringLiteral("No active transaction to rollback in database %1").arg(dbPath));
        return false;
    }

    QSqlQuery query(db);

    if (info->transactionDepth > 1)
    {
        const QString savepointName = QStringLiteral("savepoint_%1").arg(info->transactionDepth - 1);

        if (!query.exec(QStringLiteral("ROLLBACK TO SAVEPOINT %1").arg(savepointName)))
        {
            setError(QStringLiteral("Failed to rollback to savepoint in database %1: %2")
                         .arg(dbPath, query.lastError().text()));
            return false;
        }

        if (!query.exec(QStringLiteral("RELEASE SAVEPOINT %1").arg(savepointName)))
        {
            setError(QStringLiteral("Failed to release savepoint in database %1: %2")
                         .arg(dbPath, query.lastError().text()));
            return false;
        }

        info->transactionDepth--;
        return true;
    }

    if (!query.exec(QStringLiteral("ROLLBACK")))
    {
        setError(QStringLiteral("Failed to rollback transaction in database %1: %2")
                     .arg(dbPath, query.lastError().text()));
        return false;
    }

    info->inTransaction = false;
    info->transactionDepth = 0;

    return true;
}

bool SqliteDatabaseManager::executeTransaction(const QString& dbPath, const QStringList& queries)
{
    if (!beginTransaction(dbPath))
    {
        setError(QStringLiteral("Failed to begin transaction: ") + error());
        return false;
    }

    for (const QString& queryStr : queries)
    {
        if (queryStr.trimmed().isEmpty())
            continue;

        if (!executeQuery(dbPath, queryStr))
        {
            const QString err = error();
            rollbackTransaction(dbPath);
            setError(err);
            return false;
        }
    }

    if (!commitTransaction(dbPath))
    {
        const QString err = QStringLiteral("Failed to commit transaction: ") + error();
        rollbackTransaction(dbPath);
        setError(err);
        return false;
    }

    return true;
}

bool SqliteDatabaseManager::rollbackAllTransactions()
{
    const quintptr tid = currentThreadId();

    QList<InfoPtr> mine;
    {
        QMutexLocker locker(&m_mutex);
        for (const InfoPtr& info : std::as_const(m_connections))
            if (info->threadId == tid && info->inTransaction)
                mine.append(info);
    }

    bool allSuccess = true;

    for (const InfoPtr& info : std::as_const(mine))
    {
        bool ok = false;

        if (QSqlDatabase::contains(info->connectionName))
        {
            QSqlDatabase db = QSqlDatabase::database(info->connectionName, false);
            if (db.isValid() && db.isOpen())
            {
                QSqlQuery query(db);
                ok = query.exec(QStringLiteral("ROLLBACK"));
            }
        }

        if (ok)
        {
            info->inTransaction = false;
            info->transactionDepth = 0;
        }
        else
            allSuccess = false;
    }

    if (allSuccess)
        setError(QString());
    else
        setError(QStringLiteral("Failed to rollback some transactions"));

    return allSuccess;
}

}