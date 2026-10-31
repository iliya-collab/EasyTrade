#ifndef SERVICESIGNALS_HPP
#define SERVICESIGNALS_HPP
#include <QObject>

namespace Core::Service
{

class ServiceSignals : public QObject
{
    Q_OBJECT

public:

    explicit ServiceSignals(QObject* parent = nullptr) : QObject(parent) {}

signals:

    // Общий канал ошибок
    void errorOccurred(const QString& error);
    // Прогресс загрузки
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
};

}

#endif // SERVICESIGNALS_HPP
