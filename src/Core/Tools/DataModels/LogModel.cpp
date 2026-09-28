#include "LogModel.hpp"
#include <QCoreApplication>

namespace Core::Tools {

QString LogModel::levelToString(QtMsgType type)
{
    switch (type) {
    case QtInfoMsg:    return "info";
    case QtWarningMsg: return "warning";
    default:           return "error";   // Critical, Fatal
    }
}

LogModel::LogModel(QObject* parent) : QAbstractListModel(parent)
{
    m_sinkId = LogManager::instance().addSink([this](const LogEntry& e) {
        if (e.type == QtDebugMsg)
            return;

        Entry entry{ e.timeStamp, levelToString(e.type), e.category, e.message };

        QMetaObject::invokeMethod(this, [this, entry] { append(entry); }, Qt::QueuedConnection);
    });
}

LogModel::~LogModel()
{
    LogManager::instance().removeSink(m_sinkId);
}

void LogModel::append(const Entry& e)
{
    if (m_entries.size() >= kMaxEntries)
    {
        beginRemoveRows({}, 0, 0);
        if (m_entries.front().level == "error")
            --m_errorCount;
        m_entries.pop_front();
        endRemoveRows();
    }

    beginInsertRows({}, static_cast<int>(m_entries.size()), static_cast<int>(m_entries.size()));
    m_entries.push_back(e);
    endInsertRows();

    if (e.level == "error")
    {
        ++m_errorCount;
        emit errorCountChanged();
    }
}

void LogModel::clear()
{
    beginResetModel();
    m_entries.clear();
    m_errorCount = 0;
    endResetModel();
    emit errorCountChanged();
}

int LogModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : int(m_entries.size());
}

QVariant LogModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= int(m_entries.size()))
        return {};

    const Entry& e = m_entries[index.row()];

    switch (role)
    {
    case TimeRole:     return e.time;
    case LevelRole:    return e.level;
    case CategoryRole: return e.category;
    case MessageRole:  return e.message;
    }
    return {};
}

QHash<int, QByteArray> LogModel::roleNames() const
{
    return {
        {TimeRole, "time"},
        {LevelRole, "level"},
        {CategoryRole, "category"},
        {MessageRole, "message"}
    };
}

}