#ifndef IFILTER_HPP
#define IFILTER_HPP
#include "IComponent.hpp"

namespace Core::Trading
{

// Когда НЕ входить. whyNot идёт в журнал.
class IFilter : public IComponent
{
public:
    virtual bool allows(const Context&, std::string& whyNot) = 0;
};

}

#endif // IFILTER_HPP
