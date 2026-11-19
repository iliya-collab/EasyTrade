#pragma once
#include "IStreamer.hpp"
#include "Streams.hpp"
#include "PrivateStreamerSignals.hpp"

namespace Core::Streamer
{

class IPrivateStreamer : public IStreamer
{
public:

    virtual ~IPrivateStreamer() = default;

    // Доступ к своему сигнальщику
    virtual PrivateStreamerSignals* signaller() const = 0;

    virtual void init(const Api& api, bool isTestnet) = 0;

    // Отправка всех текущих подписок
    virtual void connectToStreams() = 0;
    virtual void disconnectFromStreams() = 0;

    // Подписка/отписка на потоки
    virtual void subscribe(QSet<PrivateStreams> streams) = 0;
    virtual void unsubscribe(QSet<PrivateStreams> streams) = 0;

protected:

    virtual void sendSubscriptionMessage(const QStringList& streams) = 0;
    virtual void sendUnsubscriptionMessage(const QStringList& streams) = 0;

    virtual QString createWalletStream() const = 0;
    virtual QString createOrderStream() const = 0;
    virtual QString createPositionStream() const = 0;
    virtual QString createExecutionStream() const = 0;

};

}
