#ifndef LUASANDBOX_HPP
#define LUASANDBOX_HPP
#include <sol/sol.hpp>
#include "Trading/LuaTrading/LuaScriptTypes.hpp"

namespace Core::Trading
{

class LuaSandbox
{
private:

    static constexpr int kMaxInstructionsPerCall = 1'000'000;
    static void instructionLimitHook(lua_State* L, lua_Debug*);


public:

    LuaSandbox();

    void armLimit(sol::state& lua);

    void openSandbox(sol::state& lua);
    std::expected<ScriptMetaDataTable, std::string> parseMeta(sol::state& lua, const std::string& scriptId);

};

}

#endif // LUASANDBOX_HPP
