#include <sol/sol.hpp>
#include <QByteArray>
#include <QCryptographicHash>
#include "LuaScriptTypes.hpp"
#include "Sandbox/LuaSandbox.hpp"

std::string Core::Trading::hashScript(const std::string& code)
{
    const QByteArray raw = QByteArray::fromRawData(code.data(), static_cast<int>(code.size()));
    return QCryptographicHash::hash(raw, QCryptographicHash::Sha256).toHex().toStdString();
}

std::expected<Core::Trading::ScriptMetaDataTable, std::string>
Core::Trading::readScriptMeta(const ScriptSource& src)
{
    try
    {
        sol::state lua;

        LuaSandbox sandbox;
        sandbox.openSandbox(lua);
        sandbox.armLimit(lua);

        sol::protected_function_result res =
            lua.safe_script(src.code, sol::script_pass_on_error, src.chunkName, sol::load_mode::text);

        if (!res.valid())
            return std::unexpected(sol::error(res).what());

        return sandbox.parseMeta(lua, src.id);

    }
    catch (const std::exception& e)
    {
        return std::unexpected(e.what());
    }

}

std::expected<Core::Trading::ScriptInfo, std::string>
Core::Trading::describeScript(const ScriptSource& src)
{
    ScriptInfo info;
    info.id = src.id;
    info.hash = src.hash;

    auto meta = readScriptMeta(src);

    if (!meta.has_value())
        return std::unexpected(meta.error());

    info.meta = meta.value();
    return info;

}