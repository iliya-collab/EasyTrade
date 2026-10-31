#pragma once
#include "IStreamer.hpp"
#include "Streams.hpp"
#include "MarketDataStreamerSignals.hpp"

namespace Core::Streamer
{

class IMarketDataStreamer : public IStreamer
{
public:

    virtual ~IMarketDataStreamer() = default;

    // Доступ к своему сигнальщику
    virtual MarketDataStreamerSignals* signaller() const = 0;

    virtual void init(bool isTestnet) = 0;

    // Отправка всех текущих подписок
    virtual void connectToStreams() = 0;
    virtual void disconnectFromStreams() = 0;

    // Подписка/отписка на потоки
    virtual void subscribeSymbol(const QString& symbol, QSet<PublicStreams> streams) = 0;
    virtual void unsubscribeSymbol(const QString& symbol, QSet<PublicStreams> streams) = 0;

protected:

    virtual void sendSubscriptionMessage(const QStringList& streams) = 0;
    virtual void sendUnsubscriptionMessage(const QStringList& streams) = 0;

    virtual QString createTickerStream(const QString& symbol) const = 0;
    virtual QString createOrderbookStream(const QString& symbol, int depth) const = 0;
    virtual QString createKlineStream(const QString& symbol, Tools::EnumHelpers::Interval interval) const = 0;
    virtual QString createPublicTradeStream(const QString& symbol) const = 0;

};

}