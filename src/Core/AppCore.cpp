#include "AppCore.hpp"
#include "Tools/LogManager.hpp"
#include <QVariant>
#include <QList>
#include <QVariantList>

AppCore::AppCore(QObject* parent) : QObject(parent)
{
    Core::Tools::LogManager::instance().setDebugEnabled(false);
    Core::Tools::LogManager::instance().setStdLogEnabled(false);
    Core::Tools::LogManager::instance().setLogFileEnabled(true);
    Core::Tools::LogManager::instance().setLogFile("app.log");

    m_marketState = std::make_shared<Core::MarketDataState>();
    m_marketMediator = std::make_shared<Core::MarketDataMediator>();
    m_marketService = std::make_shared<Core::MarketDataService>(m_marketState, m_marketMediator);

    m_accountMediator = std::make_shared<Core::AccountMediator>();
    m_accountState = std::make_shared<Core::AccountState>();
    m_accountService = std::make_shared<Core::AccountService>(m_accountState, m_accountMediator);

    m_logModel = std::make_unique<Core::Tools::LogModel>();

    connect(m_marketService.get(), &Core::MarketDataService::messageReceived,
            this, [](const QString& m) { qInfo().noquote() << "Market:" << m; });
    connect(m_accountService.get(), &Core::AccountService::messageReceived,
            this, [](const QString& m) { qInfo().noquote() << "Account:" << m; });

    init();
}

AppCore &AppCore::instance()
{
    static AppCore core;
    return core;
}

AppCore *AppCore::create(QQmlEngine *, QJSEngine *)
{
    auto* inst = &instance();
    QJSEngine::setObjectOwnership(inst, QJSEngine::CppOwnership);
    return inst;
}

void AppCore::init()
{   
    try
    {

        auto& configManager = Core::ConfigurationManager::instance();
        qInfo() << "Config file:" << configManager.configPath();

        auto res = configManager.tryLoad();
        if (!res.has_value())
        {
            qCritical() << res.error();
            return;
        }

        auto config = res.value_or(Core::ConfigurationParams());
        if (!config.m_apis.contains(config.m_activeApi))
        {
            qCritical() << "Active API not found:" << config.m_activeApi;
            return;
        }
        const auto& activeApi = config.m_apis.value(config.m_activeApi);

        qInfo() << "Network:" << (config.m_testnet ? "testnet" : "mainnet");
        qInfo() << "Trading:" << (config.m_enableTrades ? "enable" : "disable");

        m_marketMediator->init(config.m_testnet);
        m_accountMediator->init(activeApi, config.m_testnet);

        m_accountService->setEnableTrades(config.m_enableTrades);
        m_accountMediator->requestInfoAboutApi();
        m_accountMediator->requestAccountBalance();

        if (config.m_autoConnection)
            m_marketMediator->runStreamer();

    }
    catch (const std::exception& e)
    {
        qCritical() << e.what();
        return;
    };

}