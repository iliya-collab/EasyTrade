#pragma once
#include "Streamer/BasePrivateStreamer.hpp"

namespace Core
{

class BybitPrivateStreamer : public Streamer::BasePrivateStreamer
{
    Q_OBJECT

public:

    explicit BybitPrivateStreamer(QObject* parent = nullptr);
    ~BybitPrivateStreamer();
\
    QString id() override;
    Eh::MarketType marketType() override;
    void init(const Api& api, bool isTestnet) override;

private:

    void sendSubscriptionMessage(const QStringList& streams) override;
    void sendUnsubscriptionMessage(const QStringList& streams) override;

    QString createWalletStream() const override;
    QString createOrderStream() const override;
    QString createPositionStream() const override;
    QString createExecutionStream() const override;

};

}