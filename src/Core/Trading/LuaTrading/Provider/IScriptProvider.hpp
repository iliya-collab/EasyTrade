#ifndef ISCRIPTPROVIDER_HPP
#define ISCRIPTPROVIDER_HPP
#include "Trading/LuaTrading/LuaScriptTypes.hpp"

namespace Core::Trading
{

class IScriptProvider
{
public:
    virtual ~IScriptProvider() = default;
    virtual std::vector<ScriptInfo> list() const = 0;
    virtual std::expected<ScriptSource, std::string> load(const std::string& id) const = 0;
};

}

#endif // ISCRIPTPROVIDER_HPP
