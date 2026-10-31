#ifndef STREAMERSIGNALS_HPP
#define STREAMERSIGNALS_HPP
#include <QObject>

namespace Core::Streamer
{

class StreamerSignals : public QObject
{
    Q_OBJECT

public:

    explicit StreamerSignals(QObject* parent = nullptr) : QObject(parent) {}

signals:

    void started(const QString& id);
    void stopped(const QString& id);
    void pingMeasured(const QString& id, qint64 pingMs);
    void errorOccurred(const QString& id, const QString& error);

};

}

#endif // STREAMERSIGNALS_HPP
