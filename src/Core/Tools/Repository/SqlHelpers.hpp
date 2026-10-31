#ifndef SQLHELPERS_HPP
#define SQLHELPERS_HPP
#include <QString>
#include <QStringList>
#include <QVariant>

namespace Core::Tools::Sql
{

// Собирает INSERT ... ON CONFLICT(...) DO UPDATE из списка колонок, чтобы плейсхолдеры
// и список обновляемых полей нельзя было рассинхронизировать с колонками вручную.
//
//  columns     — все колонки вставки, в порядке биндингов
//  keyColumns  — колонки из UNIQUE-ограничения (должны точно совпадать с ним в таблице)
//  guard       — опциональное условие обновления (SQLite: DO UPDATE SET ... WHERE <guard>),
//                например защита от затирания более свежей записи устаревшей
inline QString upsert(const QString& table,
                      const QStringList& columns,
                      const QStringList& keyColumns,
                      const QString& guard = {})
{
    QStringList updates;
    for (const auto& c : columns)
        if (!keyColumns.contains(c))
            updates << QStringLiteral("%1 = excluded.%1").arg(c);

    const QString head = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3) ON CONFLICT(%4) ").arg(
        table,
        columns.join(QStringLiteral(", ")),
        QStringList(columns.size(), QStringLiteral("?")).join(QStringLiteral(", ")),
        keyColumns.join(QStringLiteral(", "))
    );

    if (updates.isEmpty())
        return head + QStringLiteral("DO NOTHING");

    QString sql = head + QStringLiteral("DO UPDATE SET ") + updates.join(QStringLiteral(", "));
    if (!guard.isEmpty())
        sql += QStringLiteral(" WHERE ") + guard;
    return sql;
}

inline QString selectFrom(const QString& table,
                          const QStringList& columns,
                          const QString& where  = {},
                          const QString& orderBy = {})
{
    QString sql = QStringLiteral("SELECT %1 FROM %2").arg(columns.join(QStringLiteral(", ")), table);

    if (!where.isEmpty())
        sql += QStringLiteral(" WHERE ") + where;

    if (!orderBy.isEmpty())
        sql += QStringLiteral(" ORDER BY ") + orderBy;

    return sql;
}

inline int placeholderCount(const QString& sql)
{
    return sql.count(QLatin1Char('?'));
}

inline bool bindingsMatch(const QString& sql, const QVariantList& bindings)
{
    return placeholderCount(sql) == bindings.size();
}

}

#endif // SQLHELPERS_HPP
