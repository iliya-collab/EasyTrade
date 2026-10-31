#ifndef MARKETDATASERVICESIGNALS_HPP
#define MARKETDATASERVICESIGNALS_HPP
#include "ServiceSignals.hpp"
#include "Tools/StdTypes.hpp"

namespace Core::Service
{

class MarketDataServiceSignals : public ServiceSignals
{
    Q_OBJECT

public:

    explicit MarketDataServiceSignals(QObject* parent = nullptr) : ServiceSignals(parent) {}

signals:

    void klinesReceived(const QList<Core::Tools::Kline>& klines);
    void allInstrumentsInfoReceived(const Core::Tools::Instruments& instruments, bool wasTruncated);

    void instrumentsInfoPageReceived(const Core::Tools::Instruments& instruments, const QString& nextPageCursor, const QVariant& context);

};

}

#endif // MARKETDATASERVICESIGNALS_HPP
