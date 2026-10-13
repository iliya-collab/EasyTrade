#ifndef MARKETDATASERVICESIGNALS_HPP
#define MARKETDATASERVICESIGNALS_HPP
#include <QObject>
#include "Tools/StdTypes.hpp"

namespace Core::Markets
{

class MarketDataServiceSignals : public QObject
{
    Q_OBJECT

public:

    explicit MarketDataServiceSignals(QObject* parent = nullptr) : QObject(parent) {}

signals:

    void errorOccurred(const QString& error);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void klinesReceived(const QList<Core::Tools::Kline>& klines);
    void tradePairsReceived(const QList<Core::Tools::TradeInfo>& pairs);

};

}

#endif // MARKETDATASERVICESIGNALS_HPP
