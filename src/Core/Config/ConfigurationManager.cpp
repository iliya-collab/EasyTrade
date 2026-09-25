#include "ConfigurationManager.hpp"
#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonDocument>

namespace Core {

    ConfigurationManager& ConfigurationManager::instance()
    {
        static ConfigurationManager s;
        return s;
    }

    QString ConfigurationManager::configPath()
    {
        return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
            + QStringLiteral("/configuration.json");
    }

    ConfigurationManager::ConfigurationManager()
    {
        if (!QFile::exists(configPath()))
        {
            generateDefaultConfiguration();
            auto res = save();
            if (!res.has_value())
                throw std::runtime_error(res.error().toStdString());
        }
    }

    void ConfigurationManager::generateDefaultConfiguration()
    {
        m_data.m_autoConnection = true;
        m_data.m_activeApi = "default";
        m_data.m_apis["default"] = { "", "", false };
    }

    std::expected<ConfigurationParams, QString> ConfigurationManager::load()
    {
        QFile file(configPath());

        if (!file.open(QIODevice::ReadOnly))
            return std::unexpected(file.errorString());

        QByteArray fileData = file.readAll();

        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(fileData);
        QJsonObject root = doc.object();

        QJsonObject apisObj = root["APIs"].toObject();
        for (auto it = apisObj.begin(); it != apisObj.end(); ++it)
        {
            QString name = it.key();
            QJsonObject apiObj = it.value().toObject();
            Tools::Api api;
            api.m_apiKey = apiObj["Key"].toString();
            api.m_secretKey = apiObj["SecretKey"].toString();
            api.m_isTestnet = apiObj["isTestnet"].toBool();
            m_data.m_apis[name] = api;
        }

        QJsonObject generalObj = root["General"].toObject();
        m_data.m_autoConnection = generalObj["AutoConnection"].toBool();
        m_data.m_activeApi = generalObj["ActiveAPI"].toString();

        if (!m_data.m_apis.contains(m_data.m_activeApi))
            m_data.m_activeApi = m_data.m_apis.isEmpty() ? QString{} : m_data.m_apis.firstKey();

        return m_data;
    }

    std::expected<void, QString> ConfigurationManager::save()
    {
        const QString dir = QFileInfo(configPath()).absolutePath();
        QDir().mkpath(dir);

        QFile file(configPath());
        if (!file.open(QIODevice::WriteOnly))
            return std::unexpected(file.errorString());

        // Корневой элемент конфигурации
        QJsonObject root;

        // Конфигурация API
        QJsonObject apisObj;
        for (auto it = m_data.m_apis.begin(); it != m_data.m_apis.end(); ++it)
        {
            QJsonObject apiObj;
            Tools::Api api = it.value();
            apiObj["Key"] = api.m_apiKey;
            apiObj["SecretKey"] = api.m_secretKey;
            apiObj["isTestnet"] = api.m_isTestnet;
            apisObj[it.key()] = apiObj;
        }
        root["APIs"] = apisObj;

        // Конфигурация General
        QJsonObject generalObj;
        generalObj["AutoConnection"] = m_data.m_autoConnection;
        generalObj["ActiveAPI"] = m_data.m_activeApi;
        root["General"] = generalObj;

        QJsonDocument doc(root);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();

        return {};
    }

    std::expected<void, QString> ConfigurationManager::addApi(const QString& name, const Tools::Api& api)
    {
        if (m_data.m_apis.contains(name))
            return std::unexpected("Such a key exists");

        m_data.m_apis[name] = api;

        return {};
    }

    std::expected<void, QString> ConfigurationManager::removeApi(const QString &name)
    {
        if (name == m_data.m_activeApi)
            return std::unexpected("Can not delete an active key");

        m_data.m_apis.remove(name);

        return {};
    }

    void ConfigurationManager::setAutoConnection(bool enabled)
    {
        m_data.m_autoConnection = enabled;
    }

    std::expected<void, QString> ConfigurationManager::setActiveApi(const QString &name)
    {
        if (!m_data.m_apis.contains(name))
            return std::unexpected("Such a key does not exist");

        m_data.m_activeApi = name;

        return {};
    }

}