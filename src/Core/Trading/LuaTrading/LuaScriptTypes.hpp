#ifndef LUASCRIPTTYPES_HPP
#define LUASCRIPTTYPES_HPP
#include <string>
#include <vector>
#include <expected>

#define API_VERSION_LUA_SCRIPTS 1

namespace Core::Trading
{

// Описание параметров индикаторов из таблицы метаданных
// Таблица meta.ind_params
struct ScriptIndParams
{
    std::string name;
    double def = 0;
    double min = 0;
    double max = 0;
    // Запрет дробных чисел
    bool integer = false;
};

// Описание параметров таблицы метаданных
// Таблица meta
struct ScriptMetaDataTable
{
    std::string title;
    int apiVersion = API_VERSION_LUA_SCRIPTS;
    std::vector<std::string> supportedMarkets;
    std::vector<ScriptIndParams> indParams;
};

// Информация о скрипте
// Запись в списке стратегий (для QML)
struct ScriptInfo
{
    std::string id;
    std::string hash;
    // Содержит meta данные скрипта, если скрипт и meta прочитались корректно
    std::expected<ScriptMetaDataTable, std::string> meta;
};

// Снимок скрипта
// LuaStrategy работает только с ним и никогда не читает файлы сама
struct ScriptSource
{
    std::string id;
    std::string code;
    std::string hash;           // SHA-256 кода (hex): записывайте в отчёты бэктеста и журнал сделок
    std::string chunkName;      // "@example.lua": в сообщениях об ошибках будет имя файла и строка
};

std::string hashScript(const std::string& code);
// Выполняет скрипт в песочнице и читает meta
std::expected<ScriptMetaDataTable, std::string> readScriptMeta(const ScriptSource& src);
// Заполняет ScriptInfo по ScriptSource
std::expected<ScriptInfo, std::string> describeScript(const ScriptSource& src);


}

#endif // LUASCRIPTTYPES_HPP
