#include "BasePrivateStreamer.hpp"

namespace Core::Streamer
{

BasePrivateStreamer::BasePrivateStreamer(std::unique_ptr<Tools::BaseWebSocket> websocket, QObject *parent)
    : BaseStreamer(std::move(websocket), parent) {}

void BasePrivateStreamer::subscribe(QSet<PrivateStreams> streams)
{
    QStringList names;
    for (auto s : streams)
        names << createStream(s);
    addStreams(names);
}

void BasePrivateStreamer::unsubscribe(QSet<PrivateStreams> streams)
{
    QStringList names;
    for (auto s : streams)
        names << createStream(s);
    removeStreams(names);
}

QString BasePrivateStreamer::createStream(PrivateStreams stream)
{
    switch (stream)
    {
    case PrivateStreams::Wallet:
        return createWalletStream();
    case PrivateStreams::Order:
        return createOrderStream();
    case PrivateStreams::Execution:
        return createExecutionStream();
    case PrivateStreams::Position:
        return createPositionStream();
    default:
        return QString();
    }
}

}