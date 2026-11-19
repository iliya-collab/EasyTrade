#include "BaseMediator.hpp"

Core::BaseMediator::BaseMediator(QObject *parent) : QObject(parent) {}

Core::BaseMediator::~BaseMediator() = default;

void Core::BaseMediator::bindStreamer(Streamer::StreamerSignals *s)
{
    connect(s, &Streamer::StreamerSignals::errorOccurred, this, &BaseMediator::errorOccurredWithId);
    connect(s, &Streamer::StreamerSignals::started,       this, &BaseMediator::streamerStarted);
    connect(s, &Streamer::StreamerSignals::stopped,       this, &BaseMediator::streamerStopped);
    connect(s, &Streamer::StreamerSignals::pingMeasured,  this, &BaseMediator::pingMeasured);
}

void Core::BaseMediator::bindService(Service::ServiceSignals *s)
{
    connect(s, &Service::ServiceSignals::errorOccurred,    this, &BaseMediator::errorOccurred);
    connect(s, &Service::ServiceSignals::downloadProgress, this, &BaseMediator::downloadProgress);
}
