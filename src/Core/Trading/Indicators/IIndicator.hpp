#ifndef IINDICATOR_HPP
#define IINDICATOR_HPP
#include "Trading/Types.hpp"

namespace Core::Trading
{

class IIndicator
{
public:
    virtual ~IIndicator() = default;

    virtual void    update(const Bar& bar) = 0;
    virtual bool    ready() const = 0;
    virtual int     outputs() const { return 1; }
    virtual double  value(int output = 0) const = 0;
    virtual void    reset() = 0;
};

}

#endif // IINDICATOR_HPP
