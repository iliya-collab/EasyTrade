#pragma once
#include "Streamer/IStreamHandler.hpp"

namespace Core::Handlers
{

    class BybitExecutionStreamHandler : public Streamer::IPrivateStreamHandler
    {
    public:

        static QString topic() { return "execution"; }

        void handle(const QString& id, const QJsonObject& data, Streamer::PrivateStreamerSignals* signaller) override;
    };

}