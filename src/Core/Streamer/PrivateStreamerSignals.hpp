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

    void accountBalanceUpdated(const Core::Tools::AccountBalance& newAccountBalance);
    void orderUpdated(const Core::Tools::OrderInfo& order);
    void executionUpdated(const Core::Tools::ExecutionInfo& exec);
    void positionUpdated(const Core::Tools::PositionInfo& position);

};

}

#endif // PRIVATESTREAMERSIGNALS_HPP
