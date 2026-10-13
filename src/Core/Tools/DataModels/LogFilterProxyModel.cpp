#include "LogFilterProxyModel.hpp"
#include "LogModel.hpp"

Core::Tools::LogFilterProxyModel::LogFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void Core::Tools::LogFilterProxyModel::setLogLevel(const QString &text)
{
    if (m_logLevel == text)
        return;

    m_logLevel = text;

    emit logLevelChanged();

    invalidate();
}

bool Core::Tools::LogFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (m_logLevel.isEmpty())
        return true;

    auto idx = sourceModel()->index(sourceRow, 0, sourceParent);
    QString lvl = sourceModel()->data(idx, LogModel::LevelRole).toString();

    return lvl.contains(m_logLevel, Qt::CaseInsensitive);
}

QString Core::Tools::LogFilterProxyModel::lineAt(int row) const
{
    const QModelIndex idx = index(row, 0);
    return QStringLiteral("[%1] %2: %3")
        .arg(data(idx, LogModel::TimeRole).toString(),
             data(idx, LogModel::CategoryRole).toString(),
             data(idx, LogModel::MessageRole).toString());
}

QString Core::Tools::LogFilterProxyModel::levelAt(int row) const
{
    return data(index(row, 0), LogModel::LevelRole).toString();
}
