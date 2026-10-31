#include "AppCore.hpp"
#include "Tools/LogManager.hpp"
#include "Tools/Logging.hpp"
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
            this, [](const QString& m) { qInfo(lcMarket).noquote() << m; });
    connect(m_marketService.get(), &Core::MarketDataService::errorOccurred,
            this, [](const QString& e) { qCritical(lcMarket).noquote() << e; });

    connect(m_accountService.get(), &Core::AccountService::messageReceived,
            this, [](const QString& m) { qInfo(lcAccount).noquote() << m; });
    connect(m_accountService.get(), &Core::AccountService::errorOccurred,
            this, [](const QString& e) { qCritical(lcAccount).noquote() << e; });

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
        coreInfo() << "Config file:" << configManager.configPath();

        auto res = configManager.tryLoad();
        if (!res.has_value())
        {
            qCritical(lcCore) << res.error();
            return;
        }

        auto config = res.value_or(Core::ConfigurationParams());
        if (!config.m_apis.contains(config.m_activeApi))
        {
            coreCrit() << "Active API not found:" << config.m_activeApi;
            return;
        }
        const auto& activeApi = config.m_apis.value(config.m_activeApi);

        coreInfo() << "Network:" << (config.m_testnet ? "testnet" : "mainnet");
        coreInfo() << "Trading:" << (config.m_enableTrades ? "enable" : "disable");

        if (!config.m_enableTrades)
            coreWarn().noquote() << "Trading was disabled";

        m_marketMediator->init(config.m_testnet);
        m_accountMediator->init(activeApi, config.m_testnet);

        m_accountService->setEnableTrades(config.m_enableTrades);
        m_accountMediator->requestInfoAboutApi();
        m_accountMediator->requestAccountBalance();

        if (config.m_autoConnection)
        {
            m_marketMediator->runStreamer();
            m_accountMediator->runStreamer();
        }

    }
    catch (const std::exception& e)
    {
        coreCrit() << e.what();
        return;
    };

}