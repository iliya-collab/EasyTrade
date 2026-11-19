#ifndef LUASTRATEGY_HPP
#define LUASTRATEGY_HPP
#include "Trading/IStrategy.hpp"
#include "LuaScriptTypes.hpp"
#include "Sandbox/LuaSandbox.hpp"
#include <memory>
#include <string>
#include <map>

namespace Core::Trading {

// class LuaStrategy final : public IStrategy
// {
// public:
//     // paramOverrides: значения для таблицы params скрипта
//     // Бросает std::runtime_error: ошибка скрипта, нет setup/on_bar/size/meta, неверный api_version, неизвестный параметр
//     LuaStrategy(ScriptSource source, InstrumentState inst,
//                 std::map<std::string, double> paramOverrides = {});
//     ~LuaStrategy() override;

//     Signal onClosedKline(const Context& ctx) override;
//     void   reset() override;                 // перезапускает ТОТ ЖЕ снимок скрипта, файл заново не читается

//     const std::string& scriptId() const;
//     const std::string& scriptHash() const;   // пишите в отчёт бэктеста и в журнал сделок
//     const std::string& lastError() const;
//     bool failed() const;                     // отключена после серии ошибок подряд

// private:
//     static InstrumentState m_inst;
//     static LuaSandbox m_luaSandbox;
//     struct Impl;
//     std::unique_ptr<Impl> m_impl;
// };


} // namespace Core::Trading

#endif