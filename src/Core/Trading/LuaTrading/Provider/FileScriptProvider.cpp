#include "FileScriptProvider.hpp"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QStandardPaths>

namespace Core::Trading
{

FileScriptProvider::FileScriptProvider() = default;

std::expected<ScriptSource, std::string>
FileScriptProvider::load(const std::string& id) const
{
    static const QRegularExpression kValidId("^[A-Za-z0-9_-]{1,64}$");

    const QString qid = QString::fromStdString(id);
    if (!kValidId.match(qid).hasMatch())
        return std::unexpected("invalid script id: " + id);

    const QString root = QDir(scriptPath()).canonicalPath();
    if (root.isEmpty())
        return std::unexpected("script directory not found: " + scriptPath().toStdString());

    const QFileInfo fi(QDir(root).filePath(qid + ".lua"));
    if (!fi.exists() || !fi.isFile())
        return std::unexpected("script not found: " + id);

    const QString canon = fi.canonicalFilePath();
    if (!canon.startsWith(root + '/'))
        return std::unexpected("script is outside of the script directory: " + id);

    QFile f(canon);
    if (!f.open(QIODevice::ReadOnly))
        return std::unexpected("cannot open script: " + id);
    const QByteArray bytes = f.readAll();
    if (bytes.size() != fi.size())
        return std::unexpected("script read error: " + id);

    const std::string code = bytes.toStdString();
    return ScriptSource{id, code, hashScript(code), "@" + id + ".lua"};
}

QString FileScriptProvider::scriptPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
        + QStringLiteral("/scripts");
}

std::vector<ScriptInfo> FileScriptProvider::list() const
{
    std::vector<ScriptInfo> out;
    const QDir dir(scriptPath());
    const QStringList files = dir.entryList({"*.lua"}, QDir::Files | QDir::Readable, QDir::Name);
    for (const QString& name : files)
    {
        const std::string id = QFileInfo(name).completeBaseName().toStdString();
        auto src = load(id);
        if (src.has_value())
        {
            auto info = describeScript(src.value());
            if (info.has_value())
                out.push_back(info.value());
            else
            {
                ScriptInfo bad;
                bad.id = id;
                bad.meta = std::unexpected(info.error());
                out.push_back(bad);
            }
        }
        else
        {
            ScriptInfo bad;
            bad.id = id;
            bad.meta = std::unexpected(src.error());
            out.push_back(bad);
        }

    }
    return out;
}

} // namespace Core::Trading