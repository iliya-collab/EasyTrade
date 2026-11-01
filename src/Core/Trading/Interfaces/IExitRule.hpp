#ifndef IEXITRULE_HPP
#define IEXITRULE_HPP
#include "IComponent.hpp"

namespace Core::Trading
{

class IExitRule : public IComponent
{
public:
    virtual double initialStopDistance(const Context&) const = 0;   // <= 0 или NaN: нельзя оценить
    virtual bool   shouldExit(const Context&, std::string& why) = 0;
};

}

#endif // IEXITRULE_HPP
