#ifndef IENTRYRULE_HPP
#define IENTRYRULE_HPP
#include "IComponent.hpp"

namespace Core::Trading
{

// Когда входить
class IEntryRule : public IComponent
{
public:
    virtual bool longSignal(const Context&) = 0;
    virtual bool shortSignal(const Context&) = 0;
};

}

#endif // IENTRYRULE_HPP
