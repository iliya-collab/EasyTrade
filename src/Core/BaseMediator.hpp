#ifndef BASEMEDIATOR_HPP
#define BASEMEDIATOR_HPP
#include "Streamer/IStreamer.hpp"
#include "Streamer/StreamerSignals.hpp"
#include "Service/ServiceSignals.hpp"
#include <QObject>

namespace Core {

class BaseMediator : public QObject
{
    Q_OBJECT
public:

    explicit BaseMediator(QObject* parent = nullptr);
    virtual ~BaseMediator();

signals:

    void errorOccurredWithId(const QString& id, const QString& error);
    void errorOccurred(const QString& error);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void streamerStarted(const QString& id);
    void streamerStopped(const QString& id);
    void pingMeasured(const QString& id, qint64 pingMs);
    void messageSent(const QString& msg);

protected:

    void bindStreamer(Streamer::StreamerSignals* s);
    void bindService(Service::ServiceSignals* s);
};

}

#endif // BASEMEDIATOR_HPP
