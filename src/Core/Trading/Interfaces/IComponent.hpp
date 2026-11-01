#ifndef ICOMPONENT_HPP
#define ICOMPONENT_HPP
#include "Trading/Types.hpp"

namespace Core::Trading
{

// Компоненты со своими индикаторами обновляются на КАЖДОЙ свече.
class IComponent
{
public:
    virtual ~IComponent() = default;
    virtual void onBar(const Bar&) {}
    virtual void reset() {}
};

}

#endif // ICOMPONENT_HPP
