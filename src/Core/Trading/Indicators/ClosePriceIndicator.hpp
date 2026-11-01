#ifndef CLOSEPRICEINDICATOR_HPP
#define CLOSEPRICEINDICATOR_HPP
#include "IIndicator.hpp"

namespace Core::Trading
{

class ClosePriceIndicator : public IIndicator
{
public:

    void update(const Bar& b) override final
    {
        updateClose(b.close);
    }

protected:

    virtual void updateClose(double close) = 0;
};

}

#endif // CLOSEPRICEINDICATOR_HPP
