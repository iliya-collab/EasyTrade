#include "ConfigurationManager.hpp"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QEventLoop>
#include <QStandardPaths>
#include <QVariantMap>

#include <qt6keychain/keychain.h>

namespace Core {

// ==================================   QtKeychain helpers  ==================================

bool ConfigurationManager::writeSecret(const QString& key, const QString& value)
{
    QKeychain::WritePasswordJob job(serviceName());
    job.setAutoDelete(false);
    job.setKey(key);
    job.setTextData(value);

    QEventLoop loop;
    QObject::connect(&job, &QKeychain::Job::finished, &loop, &QEventLoop::quit);
    job.start();
    loop.exec();

    return job.error() == QKeychain::NoError;
}

QString ConfigurationManager::readSecret(const QString& key)
{
    QKeychain::ReadPasswordJob job(serviceName());
    job.setAutoDelete(false);
    job.setKey(key);

    QEventLoop loop;
    QObject::connect(&job, &QKeychain::Job::finished, &loop, &QEventLoop::quit);
    job.start();
    loop.exec();

    return job.error() == QKeychain::NoError ? job.textData() : QString{};
}

bool ConfigurationManager::deleteSecret(const QString& key)
{
    QKeychain::DeletePasswordJob job(serviceName());
    job.setAutoDelete(false);
    job.setKey(key);

    QEventLoop loop;
    QObject::connect(&job, &QKeychain::Job::finished, &loop, &QEventLoop::quit);
    job.start();
    loop.exec();

    const auto err = job.error();
    return err == QKeychain::NoError || err == QKeychain::EntryNotFound;
}

// ==================================   Singleton  ==================================

ConfigurationManager& ConfigurationManager::instance()
{
    static ConfigurationManager s;
    return s;
}

ConfigurationManager* ConfigurationManager::create(QQmlEngine*, QJSEngine*)
{
    auto* inst = &instance();
    QJSEngine::setObjectOwnership(inst, QJSEngine::CppOwnership);
    return inst;

}

QString ConfigurationManager::configPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
    + QStringLiteral("/configuration.json");
}

ConfigurationManager::ConfigurationManager(QObject* parent)
    : QObject(parent)
{
    if (!QFile::exists(configPath()))
    {
        m_data.m_autoConnection = true;
        m_data.m_activeApi = QStringLiteral("default");
        m_data.m_testnet = true;
        m_data.m_enableTrades = false;
        m_data.m_apis[QStringLiteral("default")] = Tools::Api{ "", "" };

        auto res = trySave();
        if (!res.has_value())
            throw std::runtime_error(res.error().toStdString());
    }
    else
    {
        auto res = tryLoad();
        if (!res.has_value())
            throw std::runtime_error(res.error().toStdString());
    }
}

// ==================================   Load  ==================================

std::expected<ConfigurationParams, QString> ConfigurationManager::tryLoad()
{
    QFile file(configPath());
    if (!file.open(QIODevice::ReadOnly))
        return std::unexpected(file.errorString());

    const QByteArray fileData = file.readAll();
    file.close();

    QJsonParseError parseError{};
    const QJsonDocument doc = QJsonDocument::fromJson(fileData, &parseError);
    if (parseError.error != QJsonParseError::NoError)
        return std::unexpected(parseError.errorString());

    const QJsonObject root = doc.object();
    m_data.m_apis.clear();

    const QJsonObject apisObj = root.value("APIs").toObject();
    for (auto it = apisObj.begin(); it != apisObj.end(); ++it)
    {
        const QString name = it.key();
        Tools::Api api;
        api.m_apiKey        = readSecret(apiKeyKey(name));
        api.m_secretKey     = readSecret(secretKeyKey(name));
        m_data.m_apis[name] = api;
    }

    const QJsonObject generalObj = root.value("General").toObject();
    m_data.m_autoConnection      = generalObj.value("AutoConnection").toBool(true);
    m_data.m_activeApi           = generalObj.value("ActiveAPI").toString();
    m_data.m_testnet             = generalObj.value("Testnet").toBool(true);
    m_data.m_enableTrades        = generalObj.value("EnableTrades").toBool(false);

    if (!m_data.m_apis.contains(m_data.m_activeApi))
        m_data.m_activeApi = m_data.m_apis.isEmpty() ? QString{} : m_data.m_apis.firstKey();

    emit apisChanged();
    emit activeApiChanged();
    emit autoConnectionChanged();
    emit testnetChanged();
    emit enableTradesChanged();

    return m_data;
}

// ==================================   Save  ==================================

std::expected<void, QString> ConfigurationManager::trySave()
{
    const QString dir = QFileInfo(configPath()).absolutePath();
    QDir().mkpath(dir);

    QFile file(configPath());
    if (!file.open(QIODevice::WriteOnly))
        return std::unexpected(file.errorString());

    QJsonObject root;

    QJsonObject apisObj;
    for (auto it = m_data.m_apis.begin(); it != m_data.m_apis.end(); ++it)
    {
        const QString name = it.key();
        const Tools::Api& api = it.value();

        if (!writeSecret(apiKeyKey(name), api.m_apiKey))
            return std::unexpected(QStringLiteral("Failed to write API key '%1'").arg(name));
        if (!writeSecret(secretKeyKey(name), api.m_secretKey))
            return std::unexpected(QStringLiteral("Failed to write secret key '%1'").arg(name));

        apisObj[name] = QJsonObject{};
    }
    root["APIs"] = apisObj;

    QJsonObject generalObj;
    generalObj["AutoConnection"] = m_data.m_autoConnection;
    generalObj["ActiveAPI"]      = m_data.m_activeApi;
    generalObj["Testnet"]        = m_data.m_testnet;
    generalObj["EnableTrades"]   = m_data.m_enableTrades;
    root["General"]              = generalObj;

    const QJsonDocument doc(root);
    if (file.write(doc.toJson(QJsonDocument::Indented)) < 0)
        return std::unexpected(file.errorString());

    file.close();
    return {};
}

// ==================================   Q_INVOKABLE  ==================================

bool ConfigurationManager::reload()
{
    auto res = tryLoad();
    if (!res.has_value()) {
        emit errorOccurred(res.error());
        return false;
    }
    return true;
}

bool ConfigurationManager::saveConfig()
{
    auto res = trySave();
    if (!res.has_value()) {
        emit errorOccurred(res.error());
        return false;
    }
    return true;
}

bool ConfigurationManager::addApi(const QString& name,
                                  const QString& apiKey,
                                  const QString& secretKey)
{
    auto res = tryAddApi(name, Tools::Api{ apiKey, secretKey });
    if (!res.has_value()) {
        emit errorOccurred(res.error());
        return false;
    }
    return true;
}

bool ConfigurationManager::removeApi(const QString& name)
{
    auto res = tryRemoveApi(name);
    if (!res.has_value()) {
        emit errorOccurred(res.error());
        return false;
    }
    emit apisChanged();
    return true;
}

QVariantMap ConfigurationManager::api(const QString& name) const
{
    QVariantMap m;
    if (!m_data.m_apis.contains(name))
        return m;

    const Tools::Api& a = m_data.m_apis.value(name);
    m["apiKey"]    = a.m_apiKey;
    m["secretKey"] = a.m_secretKey;
    return m;
}

// ==================================   C++ API  ==================================

std::expected<void, QString> ConfigurationManager::tryAddApi(const QString& name, const Tools::Api& api)
{
    if (m_data.m_apis.contains(name))
        return std::unexpected(QStringLiteral("Such a key exists"));

    if (!writeSecret(apiKeyKey(name), api.m_apiKey))
        return std::unexpected(QStringLiteral("Failed to write API key to keychain"));
    if (!writeSecret(secretKeyKey(name), api.m_secretKey))
        return std::unexpected(QStringLiteral("Failed to write secret key to keychain"));

    m_data.m_apis[name] = api;
    emit apisChanged();
    return {};
}

std::expected<void, QString> ConfigurationManager::tryRemoveApi(const QString& name)
{
    if (name == m_data.m_activeApi)
        return std::unexpected(QStringLiteral("Can not delete an active key"));
    if (!m_data.m_apis.contains(name))
        return std::unexpected(QStringLiteral("Such a key does not exist"));

    deleteSecret(apiKeyKey(name));
    deleteSecret(secretKeyKey(name));
    m_data.m_apis.remove(name);

    emit apisChanged();
    return {};
}

std::expected<void, QString> ConfigurationManager::trySetActiveApi(const QString& name)
{
    if (!m_data.m_apis.contains(name))
        return std::unexpected(QStringLiteral("Such a key does not exist"));

    if (m_data.m_activeApi == name)
        return {};

    m_data.m_activeApi = name;
    emit activeApiChanged();
    return {};
}

// ==================================   Slots (Q_PROPERTY WRITE)  ==================================

void ConfigurationManager::setAutoConnection(bool enabled)
{
    if (m_data.m_autoConnection == enabled)
        return;
    m_data.m_autoConnection = enabled;
    emit autoConnectionChanged();
}

void ConfigurationManager::setEnableTrades(bool enabled)
{
    if (m_data.m_enableTrades == enabled)
        return;
    m_data.m_enableTrades = enabled;
    emit enableTradesChanged();
}

void ConfigurationManager::setTestnet(bool enabled)
{
    if (m_data.m_testnet == enabled)
        return;
    m_data.m_testnet = enabled;
    emit testnetChanged();
}

void ConfigurationManager::setActiveApi(const QString& name)
{
    auto res = trySetActiveApi(name);
    if (!res.has_value())
        emit errorOccurred(res.error());
}

} // namespace Core