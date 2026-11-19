// #include "LuaStrategy.hpp"
// #include "Trading/Indicators/Indicators.hpp"
// #include "Tools/Logging.hpp"
// #include <algorithm>
// #include <cmath>
// #include <cstdio>
// #include <stdexcept>
// #include <vector>

// namespace Core::Trading {

// namespace {

// constexpr int kMaxConsecutiveErrors = 3;   // после стольких ошибок подряд стратегия отключается

// std::string num(double v)
// {
//     char buf[32];
//     std::snprintf(buf, sizeof buf, "%.6g", v);
//     return buf;
// }

// // Индикатор: тип для Lua + глобальная функция-конструктор (Ema(20) и т.д.).
// // Объектом владеет C++ (store), Lua получает ссылку и читает значения.
// template <class T, class... Extra>
// void bindIndicator(sol::state& lua, const std::string& name,
//                    std::vector<std::unique_ptr<IIndicator>>& store, Extra&&... extra)
// {
//     lua.new_usertype<T>(name + "T", sol::no_constructor,
//                         "ready", [](const T& t) { return t.ready(); },
//                         "value", [](const T& t, sol::optional<int> out) { return t.value(out.value_or(0)); },
//                         std::forward<Extra>(extra)...);

//     lua[name] = [&store](int period) {
//         auto p = std::make_unique<T>(period);
//         T* raw = p.get();
//         store.push_back(std::move(p));
//         return raw;
//     };
// }

// } // namespace

// struct LuaStrategy::Impl
// {
//     ScriptSource src;
//     std::map<std::string, double> overrides;

//     std::unique_ptr<sol::state> lua;
//     sol::table barTbl, posTbl, indTbl, accTbl;
//     sol::protected_function onBar, sizeFn;
//     std::vector<std::unique_ptr<IIndicator>> indicators;

//     std::string lastError;
//     int  errorStreak = 0;
//     bool disabled = false;

//     // Запуск снимка скрипта
//     void load()
//     {
//         indicators.clear();
//         onBar = sol::protected_function();
//         sizeFn = sol::protected_function();
//         lua = std::make_unique<sol::state>();
//         m_luaSandbox.openSandbox(*lua);

//         bindIndicator<Sma>(*lua, "Sma", indicators);
//         bindIndicator<Ema>(*lua, "Ema", indicators);
//         bindIndicator<Rsi>(*lua, "Rsi", indicators);
//         bindIndicator<Atr>(*lua, "Atr", indicators);
//         bindIndicator<DonchianChannel>(*lua, "Donchian", indicators,
//                                 "prev_ready", [](const DonchianChannel& d) { return d.prevReady(); },
//                                 "prev_upper", [](const DonchianChannel& d) { return d.prevUpper(); },
//                                 "prev_lower", [](const DonchianChannel& d) { return d.prevLower(); });

//         barTbl = lua->create_table();
//         posTbl = lua->create_table();
//         indTbl = lua->create_table();
//         accTbl = lua->create_table();
//         accTbl["qty_step"]     = m_inst.qtyStep;
//         accTbl["min_qty"]      = m_inst.minQty;
//         accTbl["max_qty"]      = limits.maxQty;
//         accTbl["max_leverage"] = limits.maxLeverage;

//         (*lua)["floor_to_step"] = [step = limits.qtyStep](double q) {
//             return step > 0 ? std::floor(q / step + 1e-9) * step : q;
//         };

//         m_luaSandbox.armLimit(*lua);
//         sol::protected_function_result res =
//             lua->safe_script(src.code, sol::script_pass_on_error, src.chunkName, sol::load_mode::text);
//         if (!res.valid())
//             throw std::runtime_error(std::string(sol::error(res).what()));

//         auto meta = m_luaSandbox.parseMeta(*lua, src.id);
//         if (!meta.has_value())
//             throw std::runtime_error(src.id + ": " + meta.error());
//         if (meta->apiVersion != API_VERSION_LUA_SCRIPTS)
//             throw std::runtime_error(src.id + ": meta.api_version must be " + std::to_string(API_VERSION_LUA_SCRIPTS));

//         // Обязательные функции скрипта: setup (создаёт индикаторы), on_bar (решения), size (размер позиции)
//         sol::protected_function setup = (*lua)["setup"];
//         if (!setup.valid())
//             throw std::runtime_error(src.id + ": script must define setup(ind)");

//         onBar = (*lua)["on_bar"];
//         if (!onBar.valid())
//             throw std::runtime_error(src.id + ": script must define on_bar(bar, pos, ind)");

//         sizeFn = (*lua)["size"];
//         if (!sizeFn.valid())
//             throw std::runtime_error(src.id + ": script must define size(acc, bar, stop_dist, ind)");

//         buildParams(*meta);

//         m_luaSandbox.armLimit(*lua);
//         sol::protected_function_result r = setup(indTbl);
//         if (!r.valid())
//             throw std::runtime_error(src.id + ": setup() failed: " + std::string(sol::error(r).what()));

//         lastError.clear();
//         errorStreak = 0;
//         disabled = false;
//     }

//     void buildParams(const ScriptMetaDataTable& meta)
//     {
//         sol::object existing = (*lua)["params"];
//         if (existing.get_type() != sol::type::lua_nil)
//             throw std::runtime_error(src.id + ": do not define global 'params': it is built from meta.params");

//         sol::table params = lua->create_table();
//         auto put = [&](const ScriptIndParams& p, double v)
//         {
//             if (p.integer)
//                 params[p.name] = static_cast<lua_Integer>(std::llround(v));
//             else
//                 params[p.name] = v;
//         };

//         for (const ScriptIndParams& p : meta.indParams)
//             put(p, p.def);

//         for (const auto& [key, value] : overrides)
//         {
//             auto it = std::find_if(meta.indParams.begin(), meta.indParams.end(),
//                                    [&](const ScriptIndParams& p) { return p.name == key; });

//             if (it == meta.indParams.end())
//                 throw std::runtime_error(src.id + ": unknown param '" + key + "'");

//             if (!std::isfinite(value) || value < it->min || value > it->max)
//                 throw std::runtime_error(src.id + ": param '" + key + "' = " + num(value) +
//                                          " is outside [" + num(it->min) + ", " + num(it->max) + "]");

//             if (it->integer && value != std::floor(value))
//                 throw std::runtime_error(src.id + ": param '" + key + "' must be an integer");

//             put(*it, value);
//         }

//         (*lua)["params"] = params;
//     }

//     void fillBarTable(const Bar& b)
//     {
//         barTbl["start"]  = static_cast<lua_Integer>(b.start);
//         barTbl["open"]   = b.open;
//         barTbl["high"]   = b.high;
//         barTbl["low"]    = b.low;
//         barTbl["close"]  = b.close;
//         barTbl["volume"] = b.volume;
//     }

//     void fillPosTable(const Position& p)
//     {
//         const char* side = p.side == SideType::Long ? "long" : p.side == SideType::Short ? "short" : "none";
//         posTbl["open"]      = p.open();
//         posTbl["side"]      = side;
//         posTbl["entry"]     = p.entry;
//         posTbl["qty"]       = p.qty;
//         posTbl["stop"]      = p.stop;
//         posTbl["highest"]   = p.highest;
//         posTbl["lowest"]    = p.lowest;
//         posTbl["bars_held"] = p.barsHeld;
//     }

//     static Signal skip(const std::string& why) { return {SignalType::None, 0.0, 0.0, "Skip: " + why}; }

//     double computeQty(const Context& ctx, double stopDist, std::string& note)
//     {
//         accTbl["equity"] = ctx.equity;
//         m_luaSandbox.armLimit(*lua);
//         sol::protected_function_result r = sizeFn(accTbl, barTbl, stopDist, indTbl);
//         if (!r.valid())
//         {
//             registerError(std::string("size(): ") + sol::error(r).what());
//             note = "size() failed";
//             return 0.0;
//         }

//         sol::optional<double> requested = r.get<sol::optional<double>>();
//         if (!requested || !std::isfinite(*requested) || !(*requested > 0)) {
//             note = "size() returned no positive number";
//             return 0.0;
//         }

//         double q = *requested;
//         q = std::min(q, ctx.equity * limits.maxLeverage * (1.0 - limits.feeReserve) / ctx.bar.close);
//         if (limits.maxQty > 0) q = std::min(q, limits.maxQty);
//         if (limits.qtyStep > 0) q = std::floor(q / limits.qtyStep + 1e-9) * limits.qtyStep;
//         if (!(q > 0) || q < limits.minQty) {
//             note = "qty " + num(q) + " below min " + num(limits.minQty);
//             return 0.0;
//         }

//         note = "requested " + num(*requested) + " -> " + num(q);
//         return q;
//     }

//     Signal toSignal(sol::protected_function_result& r, const Context& ctx)
//     {
//         if (r.return_count() == 0)
//             return {};
//         sol::object obj = r.get<sol::object>();
//         if (obj.get_type() != sol::type::table)
//             return {};

//         sol::table t = obj.as<sol::table>();
//         sol::optional<std::string> action = t["action"];
//         sol::optional<std::string> reasonOpt = t["reason"];
//         sol::optional<double> stopOpt = t["stop"];
//         const std::string reason = reasonOpt.value_or("");

//         if (!action || *action == "none")
//             return reason.empty() ? Signal{} : skip(reason);

//         if (*action == "close") {
//             if (!ctx.pos.open())
//                 return skip("close without open position");
//             return {SignalType::Close, ctx.pos.qty, 0.0, reason};
//         }

//         if (*action == "buy" || *action == "sell") {
//             if (ctx.pos.open())
//                 return skip("entry signal while in position");

//             const bool isLong = (*action == "buy");
//             const double price = ctx.bar.close;
//             const double stop = stopOpt.value_or(0.0);

//             if (!(stop > 0) || (isLong && !(stop < price)) || (!isLong && !(stop > price)))
//                 return skip("invalid stop " + std::to_string(stop));

//             std::string note;
//             const double qty = computeQty(ctx, std::abs(price - stop), note);
//             if (!(qty > 0))
//                 return skip(note);

//             const std::string base = reason.empty() ? (isLong ? "Long entry" : "Short entry") : reason;
//             return {isLong ? SignalType::OpenLong : SignalType::OpenShort, qty, stop, base + " (" + note + ")"};
//         }

//         registerError("unknown action '" + *action + "'");
//         return skip("unknown action");
//     }

//     void registerError(const std::string& msg)
//     {
//         lastError = msg;
//         coreCrit().noquote() << "[lua error]"
//                              << QString::fromStdString(src.id)
//                              << QString::fromStdString(msg);
//         if (++errorStreak >= kMaxConsecutiveErrors)
//             disabled = true;
//     }
// };

// LuaSandbox LuaStrategy::m_luaSandbox{};

// LuaStrategy::LuaStrategy(ScriptSource source, InstrumentLimits limits,
//                          std::map<std::string, double> paramOverrides)
//     : m_impl(std::make_unique<Impl>())
// {
//     m_impl->src = std::move(source);
//     m_impl->limits = limits;
//     m_impl->overrides = std::move(paramOverrides);
//     m_impl->load();
// }

// LuaStrategy::~LuaStrategy() = default;

// Signal LuaStrategy::onClosedKline(const Context& ctx)
// {
//     Impl& d = *m_impl;
//     if (d.disabled)
//         return {SignalType::None, 0.0, 0.0, "Error: script disabled after repeated failures"};

//     for (auto& i : d.indicators)           // индикаторы обновляет C++, до вызова скрипта
//         i->update(ctx.bar);

//     d.fillBarTable(ctx.bar);
//     d.fillPosTable(ctx.pos);

//     m_luaSandbox.armLimit(*d.lua);
//     sol::protected_function_result r = d.onBar(d.barTbl, d.posTbl, d.indTbl);
//     if (!r.valid()) {
//         d.registerError(sol::error(r).what());
//         return {SignalType::None, 0.0, 0.0, "Error: " + d.lastError};   // ошибка = нет сделки
//     }
//     d.errorStreak = 0;
//     return d.toSignal(r, ctx);
// }

// void LuaStrategy::reset() { m_impl->load(); }

// const std::string& LuaStrategy::scriptId() const   { return m_impl->src.id; }
// const std::string& LuaStrategy::scriptHash() const { return m_impl->src.hash; }
// const std::string& LuaStrategy::lastError() const  { return m_impl->lastError; }
// bool LuaStrategy::failed() const                   { return m_impl->disabled; }

// InstrumentState LuaStrategy::m_inst{};

// } // namespace Core::Trading