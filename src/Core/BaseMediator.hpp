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
    explicit BaseMediator(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~BaseMediator() = default;

    void runStreamer()       { if (auto* s = streamer()) s->start(); }
    void stopStreamer()      { if (auto* s = streamer()) s->stop(); }
    void restartStreamer()   { if (auto* s = streamer()) s->restart(); }
    bool isStreamerRunning() { auto* s = streamer(); return s && s->isRunning(); }

signals:
    void errorOccurredWithId(const QString& id, const QString& error);
    void errorOccurred(const QString& error);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void streamerStarted(const QString& id);
    void streamerStopped(const QString& id);
    void pingMeasured(const QString& id, qint64 pingMs);
    void messageSent(const QString& msg);

protected:
    virtual Streamer::IStreamer* streamer() = 0;

    void bindStreamer(Streamer::StreamerSignals* s)
    {
        connect(s, &Streamer::StreamerSignals::errorOccurred, this, &BaseMediator::errorOccurredWithId);
        connect(s, &Streamer::StreamerSignals::started,       this, &BaseMediator::streamerStarted);
        connect(s, &Streamer::StreamerSignals::stopped,       this, &BaseMediator::streamerStopped);
        connect(s, &Streamer::StreamerSignals::pingMeasured,  this, &BaseMediator::pingMeasured);
    }

    void bindService(Service::ServiceSignals* s)
    {
        connect(s, &Service::ServiceSignals::errorOccurred,    this, &BaseMediator::errorOccurred);
        connect(s, &Service::ServiceSignals::downloadProgress, this, &BaseMediator::downloadProgress);
    }
};

}

#endif // BASEMEDIATOR_HPP
