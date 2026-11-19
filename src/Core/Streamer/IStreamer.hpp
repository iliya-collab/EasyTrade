#pragma once
#include <QString>
#include "Tools/StdTypes.hpp"

namespace Core::Streamer
{

class IStreamer
{
public:

    explicit IStreamer() = default;
    virtual ~IStreamer() = default;

    virtual Eh::MarketType marketType() = 0;
    virtual QString id() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void restart() = 0;
    virtual bool isRunning() = 0;

};

}
