#ifndef LINEARTRACKER_HPP
#define LINEARTRACKER_HPP
#include "Trading/Common/IPositionTracker.hpp"

namespace Core::Trading
{

class LinearTracker final : IPositionTracker
{
public:

    explicit LinearTracker();

    void onOrder(const OrderState& o) override;;
    void onExec(const ExecutionState& e) override;;
    void onBarClosed(const Bar& b) override;
    void setStop(double stop) override;
    void reset() override;
    AnyPosition snapshot() const override;

    void onPos(const PositionState& p);

private:

    LinearPosition m_pos;

};

}


#endif // LINEARTRACKER_HPP
