#ifndef PRIVATESTREAMERSIGNALS_HPP
#define PRIVATESTREAMERSIGNALS_HPP
#include "StreamerSignals.hpp"
#include "Tools/StdTypes.hpp"

namespace Core::Streamer
{

class PrivateStreamerSignals : public StreamerSignals
{
    Q_OBJECT

public:

    explicit PrivateStreamerSignals(QObject* parent = nullptr) : StreamerSignals(parent) {}

signals:

    void walletUpdated(const WalletInfo& wallet);
    void orderUpdated(const OrderInfo& order);
    void executionUpdated(const ExecutionInfo& exec);
    void positionUpdated(const PositionInfo& position);

};

}

#endif // PRIVATESTREAMERSIGNALS_HPP
