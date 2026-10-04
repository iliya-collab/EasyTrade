#pragma once
#include <QSortFilterProxyModel>
#include <qqmlregistration.h>

namespace Core::Tools {

class LogFilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString logLevel READ getLogLevel WRITE setLogLevel NOTIFY logLevelChanged FINAL)

public:

    explicit LogFilterProxyModel(QObject* parent = nullptr);

    QString getLogLevel() const { return m_logLevel; }
    void setLogLevel(const QString &text);

signals:
    void logLevelChanged();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:

    QString m_logLevel = "";

};

}

