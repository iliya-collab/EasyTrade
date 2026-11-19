#include "MultiMediator.hpp"

Core::MultiMediator::MultiMediator(QObject *parent) : BaseMediator(parent) {}

Core::MultiMediator::~MultiMediator() = default;

void Core::MultiMediator::runAllStreams()
{
    forEachStreamer([](auto* s) { s->start(); });
}

void Core::MultiMediator::stopAllStreams()
{
    forEachStreamer([](auto* s) { s->stop(); });
}

void Core::MultiMediator::restartAllStreams()
{
    forEachStreamer([](auto* s) { s->restart(); });
}

void Core::MultiMediator::runStream(Tools::Types::EnumHelpers::MarketType market)
{
    if (auto* s = streamerFor(market))
        s->start();
}

void Core::MultiMediator::stopStream(Tools::Types::EnumHelpers::MarketType market)
{
    if (auto* s = streamerFor(market))
        s->stop();
}

void Core::MultiMediator::restartStream(Tools::Types::EnumHelpers::MarketType market)
{
    if (auto* s = streamerFor(market))
        s->restart();
}

bool Core::MultiMediator::isStreamerRunning(Tools::Types::EnumHelpers::MarketType market) const
{
    auto* s = streamerFor(market);
    return s && s->isRunning();
}

QList<Core::Streamer::IStreamer*> Core::MultiMediator::allStreamers() const
{
    QList<Streamer::IStreamer*> out;
    for (auto* s : cpool().streams())
        out.push_back(s);
    return out;
}

Core::Streamer::IStreamer* Core::MultiMediator::streamerFor(Tools::Types::EnumHelpers::MarketType market) const
{
    return cpool()[market];
}