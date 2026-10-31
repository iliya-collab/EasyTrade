#pragma once
#include <QString>

namespace Core::Streamer
{

class IStreamer
{
public:

    explicit IStreamer() = default;
    virtual ~IStreamer() = default;

    virtual QString id() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void restart() = 0;
    virtual bool isRunning() = 0;

};

}
