#pragma once
#include "Streamer/IStreamHandler.hpp"

namespace Core::Handlers
{

class BybitOrderStreamHandler : public Streamer::IPrivateStreamHandler
{
public:

    static QString topic() { return "order"; }

    void handle(const QString& id, const QJsonObject& data, Streamer::PrivateStreamerSignals* signaller) override;
};

}


