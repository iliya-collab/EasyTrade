#ifndef IPOSITIONTRACKER_HPP
#define IPOSITIONTRACKER_HPP
#include "Types.hpp"

namespace Core::Trading
{

class IPositionTracker
{
public:
    virtual ~IPositionTracker() = default;
    virtual void onBarClosed(const Bar& b) = 0;
    virtual void onOrder(const OrderState& s) = 0;
    virtual void onExec(const ExecutionState& s) = 0;
    virtual void setStop(double stop) = 0;
    virtual void reset() = 0;
    virtual AnyPosition snapshot() const = 0;
};

}

#endif // IPOSITIONTRACKER_HPP
