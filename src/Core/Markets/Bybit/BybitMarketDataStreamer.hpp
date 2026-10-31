#pragma once
#include "Streamer/BaseMarketDataStreamer.hpp"
 
namespace Core
{

class BybitMarketDataStreamer : public Streamer::BaseMarketDataStreamer
{
    Q_OBJECT

public:

    explicit BybitMarketDataStreamer(Tools::EnumHelpers::MarketType type, QObject* parent = nullptr);
    ~BybitMarketDataStreamer();

    QString id() override;
    void init(bool isTestnet) override;

private:

    void sendSubscriptionMessage(const QStringList& streams) override;
    void sendUnsubscriptionMessage(const QStringList& streams) override;

    QString createTickerStream(const QString& symbol) const override;
    QString createOrderbookStream(const QString& symbol, int depth) const override;
    QString createKlineStream(const QString& symbol, Tools::EnumHelpers::Interval interval) const override;
    QString createPublicTradeStream(const QString& symbol) const override;

};

}