#ifndef ISIZER_HPP
#define ISIZER_HPP
#include "Trading/Common/Types.hpp"

namespace Core::Trading
{

// Сколько брать
class ISizer
{
public:
    virtual ~ISizer() = default;
    virtual double qty(const Context&, double stopDistance) = 0;
};

}

#endif // ISIZER_HPP
