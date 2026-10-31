#pragma once
#include "Streamer/IStreamHandler.hpp"

namespace Core::Handlers
{

    class BybitPositionStreamHandler : public Streamer::IPrivateStreamHandler
    {
    public:

        static QString topic() { return "position"; }

        void handle(const QString& id, const QJsonObject& data, Streamer::PrivateStreamerSignals* signaller) override;
    };

}
