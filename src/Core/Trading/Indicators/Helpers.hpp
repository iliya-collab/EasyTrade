#ifndef HELPERS_HPP
#define HELPERS_HPP
#include <limits>
#include <stdexcept>


namespace Core::Trading
{

inline int checkPeriod(int p)
{
    if (p < 1)
        throw std::invalid_argument("indicator period must be >= 1");
    return p;
}

constexpr double kNaN = std::numeric_limits<double>::quiet_NaN();

}

#endif // HELPERS_HPP
