#ifndef MULTIMEDIATOR_HPP
#define MULTIMEDIATOR_HPP
#include "BaseMediator.hpp"
#include "Core/ConnectionsPool.hpp"
#include "Tools/Types/EnumHelpers.hpp"

namespace Core {

class MultiMediator : public BaseMediator
{
    Q_OBJECT
public:

    explicit MultiMediator(QObject* parent = nullptr);
    ~MultiMediator() override;

    // Уравление всеми соединениями
    void runAllStreams();
    void stopAllStreams();
    void restartAllStreams();

    // Уравление конкретными соединениями
    void runStream(Tools::Types::EnumHelpers::MarketType market);
    void stopStream(Tools::Types::EnumHelpers::MarketType market);
    void restartStream(Tools::Types::EnumHelpers::MarketType market);
    bool isStreamerRunning(Tools::Types::EnumHelpers::MarketType market) const;

protected:

    virtual ConnectionsPool& pool() = 0;
    virtual const ConnectionsPool& cpool() const = 0;

    Streamer::IStreamer* streamerFor(Tools::Types::EnumHelpers::MarketType market) const;
    QList<Streamer::IStreamer*> allStreamers() const;

    template<typename Fn>
    void forEachStreamer(Fn&& fn)
    {
        const auto& all = allStreamers();
        for (auto* s : all)
            if (s) fn(s);
    }

};

}

#endif // MULTIMEDIATOR_HPP
