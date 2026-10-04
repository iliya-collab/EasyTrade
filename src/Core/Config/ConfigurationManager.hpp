#pragma once
#include "Configuration.hpp"

#include <QObject>
#include <QQmlEngine>
#include <QStringList>
#include <QVariantList>
#include <expected>

namespace Core {

class ConfigurationManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(bool         autoConnection   READ autoConnection     WRITE setAutoConnection     NOTIFY autoConnectionChanged   FINAL)
    Q_PROPERTY(bool         enableTrades     READ enableTrades       WRITE setEnableTrades       NOTIFY enableTradesChanged     FINAL)
    Q_PROPERTY(bool         testnet          READ testnet            WRITE setTestnet            NOTIFY testnetChanged          FINAL)
    Q_PROPERTY(QString      activeApi        READ activeApi          WRITE setActiveApi          NOTIFY activeApiChanged        FINAL)
    Q_PROPERTY(QStringList  apiNames         READ apiNames                                       NOTIFY apisChanged             FINAL)

public:

    // --- Singleton для QML ---
    static ConfigurationManager& instance();
    static ConfigurationManager* create(QQmlEngine*, QJSEngine*);

    static QString configPath();

    // --- Q_PROPERTY getters ---
    bool        autoConnection() const noexcept { return m_data.m_autoConnection; }
    bool        testnet()        const noexcept { return m_data.m_testnet; }
    bool        enableTrades()   const noexcept { return m_data.m_enableTrades; }
    QString     activeApi()      const noexcept { return m_data.m_activeApi; }
    QStringList apiNames()       const noexcept { return m_data.m_apis.keys(); }

    // --- Q_PROPERTY setters ---
    void setAutoConnection(bool enabled);
    void setEnableTrades(bool enabled);
    void setTestnet(bool enabled);
    void setActiveApi(const QString& name);

    // --- Q_INVOKABLE для QML ---
    Q_INVOKABLE bool addApi(const QString& name, const QString& apiKey, const QString& secretKey);
    Q_INVOKABLE bool removeApi(const QString& name);
    Q_INVOKABLE QVariantMap api(const QString& name) const;
    Q_INVOKABLE bool reload();
    Q_INVOKABLE bool saveConfig();

    const ConfigurationParams& data() const noexcept { return m_data; }
    std::expected<ConfigurationParams, QString> tryLoad();
    std::expected<void, QString> trySave();
    std::expected<void, QString> tryAddApi(const QString& name, const Tools::Api& api);
    std::expected<void, QString> tryRemoveApi(const QString& name);
    std::expected<void, QString> trySetActiveApi(const QString& name);

signals:

    void autoConnectionChanged();
    void testnetChanged();
    void enableTradesChanged();
    void activeApiChanged();
    void apisChanged();

    void errorOccurred(const QString& error);

private:

    explicit ConfigurationManager(QObject* parent = nullptr);
    ConfigurationManager(const ConfigurationManager&) = delete;
    ConfigurationManager& operator=(const ConfigurationManager&) = delete;

    // QtKeychain helpers
    static QString serviceName() { return QStringLiteral("EasyTrade"); }
    static QString apiKeyKey(const QString& name)    { return QStringLiteral("apiKey_%1").arg(name); }
    static QString secretKeyKey(const QString& name) { return QStringLiteral("secretKey_%1").arg(name); }

    static bool    writeSecret (const QString& key, const QString& value);
    static QString readSecret  (const QString& key);
    static bool    deleteSecret(const QString& key);

    ConfigurationParams m_data;
};

}
