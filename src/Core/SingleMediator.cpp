#include "SingleMediator.hpp"

Core::SingleMediator::SingleMediator(QObject *parent) : BaseMediator(parent) {}

Core::SingleMediator::~SingleMediator() = default;

void Core::SingleMediator::runStreamer()
{
    if (auto* s = streamer())
        s->start();
}

void Core::SingleMediator::stopStreamer()
{
    if (auto* s = streamer())
        s->stop();
}

void Core::SingleMediator::restartStreamer()
{
    if (auto* s = streamer())
        s->restart();
}

bool Core::SingleMediator::isStreamerRunning()
{
    auto* s = streamer();
    return s && s->isRunning();
}
