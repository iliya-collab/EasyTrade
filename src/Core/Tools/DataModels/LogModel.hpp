#pragma once
#include <QAbstractListModel>
#include <QDateTime>
#include <QQmlEngine>
#include <deque>
#include "Tools/LogManager.hpp"

namespace Core::Tools {

class LogModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ANONYMOUS

    Q_PROPERTY(int errorCount READ errorCount NOTIFY errorCountChanged FINAL)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)

public:

    enum Roles {
        TimeRole = Qt::UserRole + 1,
        LevelRole,
        CategoryRole,
        MessageRole
    };

    explicit LogModel(QObject* parent = nullptr);
    ~LogModel() override;

    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    int errorCount() const { return m_errorCount; }
    int count() const { return m_count; }

    Q_INVOKABLE void clear();

signals:

    void errorCountChanged();
    void countChanged();

private:

    struct Entry {
        QString time{};
        QString level{};
        QString category{};
        QString message{};
    };

    void append(const Entry& e);
    static QString levelToString(QtMsgType type);

    static constexpr int kMaxEntries = 2000;
    std::deque<Entry> m_entries{};
    int m_errorCount = 0;
    int m_count = 0;
    int m_sinkId = -1;

};

}
