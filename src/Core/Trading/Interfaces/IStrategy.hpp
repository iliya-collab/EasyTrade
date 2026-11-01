#ifndef ISTRATEGY_HPP
#define ISTRATEGY_HPP
#include "Trading/Types.hpp"

namespace Core::Trading
{

// То, что видят движок и бэктестер. Остальное им знать не нужно
class IStrategy
{
public:
    virtual ~IStrategy() = default;
    virtual Signal onClosedKline(const Context&) = 0;
    virtual void   reset() = 0;
};

}

#endif // ISTRATEGY_HPP
