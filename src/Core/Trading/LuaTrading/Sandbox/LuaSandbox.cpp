#include "LuaSandbox.hpp"
#include <algorithm>
#include "Tools/Logging.hpp"


void Core::Trading::LuaSandbox::instructionLimitHook(lua_State *L, lua_Debug *)
{
    luaL_error(L, "instruction limit exceeded");
}

Core::Trading::LuaSandbox::LuaSandbox() = default;

void Core::Trading::LuaSandbox::armLimit(sol::state &lua)
{
    lua_sethook(lua.lua_state(), instructionLimitHook, LUA_MASKCOUNT, kMaxInstructionsPerCall);
}

void Core::Trading::LuaSandbox::openSandbox(sol::state &lua)
{
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);

    for (const char* name : {"dofile", "loadfile", "load", "loadstring"})
        lua[name] = sol::lua_nil;

    lua["log"] = [](const std::string& s) { coreInfo().noquote() << "[lua]" << QString::fromStdString(s); };
    lua["print"] = lua["log"];

}

std::expected<Core::Trading::ScriptMetaDataTable, std::string>
Core::Trading::LuaSandbox::parseMeta(sol::state &lua, const std::string &scriptId)
{
    sol::optional<sol::table> meta = lua["meta"];
    if (!meta)
        return std::unexpected("script must define a 'meta' table");

    sol::optional<std::string> title = (*meta)["title"];
    sol::optional<int> ver = (*meta)["api_version"];
    if (!ver)
        return std::unexpected("meta.api_version is missing");

    ScriptMetaDataTable m;
    m.title = title.value_or(scriptId);
    m.apiVersion = *ver;

    sol::optional<sol::table> markets = (*meta)["markets"];
    if (!markets)
        return std::unexpected("meta.markets is missing");

    sol::table markets_table = markets.value();

    for (auto& [key, value] : markets_table)
        m.supportedMarkets.emplace_back(value.as<std::string>());

    sol::optional<sol::table> params = (*meta)["ind_params"];
    if (params)
    {
        std::string problem;
        params->for_each([&](sol::object key, sol::object value)
        {
            if (!problem.empty())
                return;

            if (key.get_type() != sol::type::string || value.get_type() != sol::type::table)
            {
                problem = "meta.ind_params must map parameter names to tables";
                return;
            }

            const std::string name = key.as<std::string>();
            sol::table t = value.as<sol::table>();
            sol::optional<double> d = t["default"], lo = t["min"], hi = t["max"];
            sol::optional<bool> isInt = t["integer"];

            if (!d || !lo || !hi)
            {
                problem = "param '" + name + "': default, min and max are required";
                return;
            }

            if (*lo > *hi || *d < *lo || *d > *hi) {
                problem = "param '" + name + "': need min <= default <= max";
                return;
            }
            m.indParams.push_back({name, *d, *lo, *hi, isInt.value_or(false)});
        });

        if (!problem.empty())
            return std::unexpected(problem);

        std::sort(m.indParams.begin(), m.indParams.end(),
                  [](const ScriptIndParams& a, const ScriptIndParams& b) { return a.name < b.name; });
    }

    return m;
}
