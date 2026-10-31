#pragma once
#include "Streamer/IStreamHandler.hpp"

namespace Core::Handlers
{

class BybitWalletStreamHandler : public Streamer::IPrivateStreamHandler
{
public:

    static QString topic() { return "wallet"; }

    void handle(const QString& id, const QJsonObject& data, Streamer::PrivateStreamerSignals* signaller) override;

};

}