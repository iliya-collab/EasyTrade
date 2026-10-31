#pragma once
#include "MarketDataStreamerSignals.hpp"
#include "PrivateStreamerSignals.hpp"
#include <QJsonObject>

namespace Core::Streamer
{

    template <typename TStreamerSignaller>
    class IStreamHandler
    {
    public:
        virtual ~IStreamHandler() = default;
        virtual void handle(const QString& id, const QJsonObject& data, TStreamerSignaller* signaller) = 0;
    };

    using IMarketDataStreamHandler = IStreamHandler<MarketDataStreamerSignals>;
    using IPrivateStreamHandler = IStreamHandler<PrivateStreamerSignals>;
}
