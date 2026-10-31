#include "DatabasePathProvider.hpp"
#include <QDir>
#include <QFileInfo>

static QString fileNameFor(const Core::Tools::dbDescriptor& d)
{
    const QString env = d.testnet ? QLatin1String("testnet") : QLatin1String("mainnet");
    switch (d.scope)
    {
    case Core::Tools::dbScope::Market:
        return QStringLiteral("/db/public/market_data_%1.db").arg(env);
    case Core::Tools::dbScope::Account:
        return QStringLiteral("/db/private/account_data_%1_%2.db").arg(d.userId, env);
    }
    return {};
}

Core::Tools::DatabasePathProvider::DatabasePathProvider(QString baseDir)
    : m_baseDir(std::move(baseDir)) {}

std::expected<QString, QString> Core::Tools::DatabasePathProvider::pathFor(const dbDescriptor &d) const
{
    auto valid = validate(d);
    if (!valid.has_value())
        return std::unexpected(valid.error());


    const QString relativePath = fileNameFor(d);
    if (relativePath.isEmpty())
        return std::unexpected(QStringLiteral("Unknown database scope"));


    QDir dir(m_baseDir);
    const QString absoluteBase = dir.absolutePath();

    QString absolutePath = absoluteBase + relativePath;
    QFileInfo fileInfo(absolutePath);


    QDir dirToCreate(fileInfo.absolutePath());
    if (!dirToCreate.exists() && !dirToCreate.mkpath(QStringLiteral(".")))
        return std::unexpected(QStringLiteral("Failed to create directory: %1").arg(dirToCreate.absolutePath()));

    return absolutePath;
}


std::expected<void, QString> Core::Tools::DatabasePathProvider::validate(const dbDescriptor &d) const
{
    if (m_baseDir.isEmpty())
        return std::unexpected("Base dir is empty");

    if (d.scope == dbScope::Account && d.userId.trimmed().isEmpty())
        return std::unexpected("Private db requires user id");

    return {};
}
