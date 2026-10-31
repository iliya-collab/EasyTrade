#pragma once
#include "BaseStreamer.hpp"
#include "IPrivateStreamer.hpp"

namespace Core::Streamer
{

    class BasePrivateStreamer : public BaseStreamer<PrivateStreamerSignals, IPrivateStreamer>
    {
        Q_OBJECT
    public:

        explicit BasePrivateStreamer(std::unique_ptr<Tools::BaseWebSocket> websocket, QObject* parent = nullptr);

        void subscribe(QSet<PrivateStreams> streams) override;
        void unsubscribe(QSet<PrivateStreams> streams) override;

    protected:

        QString createStream(PrivateStreams stream);

    };

}