#pragma once
#include <QObject>
#include <QMessageLogContext>
#include <QString>
#include <QFile>
#include <QMutex>

namespace Core::Tools {

struct LogEntry
{
    QtMsgType type;
    QString category;
    QString message;
    QString timeStamp;
};

using LogSink = std::function<void(const LogEntry&)>;

class LogManager
{
public:

    static LogManager& instance();

    void setDebugEnabled(bool enabled);
    void setInfoEnabled(bool enabled);
    void setWarningEnabled(bool enabled);

    void setLogFile(const QString& name);
    void setLogFileEnabled(bool enabled);
    void setStdLogEnabled(bool enanbled);

    bool isDebugEnabled() const;
    bool isInfoEnabled() const;
    bool isWarningEnabled() const;
    bool isLogFileEnabled() const;
    bool isStdLogEnabled() const;

    // Подписка на поток сообщений
    int addSink(LogSink sink);
    void removeSink(int id);

private:

    LogManager();
    ~LogManager() = default;

    LogManager(const LogManager&) = delete;
    LogManager& operator= (const LogManager&) = delete;

    static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

    bool m_debugEnabled = true;
    bool m_infoEnabled = true;
    bool m_warningEnabled = true;

    bool m_isLogFile = false;
    bool m_isStdLog = true;

    QFile m_logFile{};
    mutable QMutex m_mutex;

    std::vector<std::pair<int, LogSink>> m_sinks;
    int m_nextSinkId = 0;
};

}
