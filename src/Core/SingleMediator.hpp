#ifndef SINGLEMEDIATOR_HPP
#define SINGLEMEDIATOR_HPP
#include "BaseMediator.hpp"

namespace Core {

class SingleMediator : public BaseMediator
{
    Q_OBJECT
public:

    explicit SingleMediator(QObject* parent = nullptr);
    ~SingleMediator() override;

    void runStreamer();
    void stopStreamer();
    void restartStreamer();
    bool isStreamerRunning();

protected:

    virtual Streamer::IStreamer* streamer() = 0;

};

}

#endif // SINGLEMEDIATOR_HPP
