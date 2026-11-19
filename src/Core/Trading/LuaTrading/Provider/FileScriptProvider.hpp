#ifndef FILESCRIPTPROVIDER_HPP
#define FILESCRIPTPROVIDER_HPP
#include "IScriptProvider.hpp"
#include <QString>

namespace Core::Trading
{

class FileScriptProvider final : public IScriptProvider
{
public:
    explicit FileScriptProvider();

    std::vector<ScriptInfo> list() const override;
    std::expected<ScriptSource, std::string> load(const std::string& id) const override;

    static QString scriptPath();

};


}

#endif // FILESCRIPTPROVIDER_HPP
