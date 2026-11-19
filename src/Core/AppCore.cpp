#include "AppCore.hpp"
#include "Config/ConfigurationManager.hpp"
#include "Tools/LogManager.hpp"
#include "Tools/Logging.hpp"
#include <QVariant>
#include <QList>
#include <QVariantList>

AppCore::AppCore(QObject* parent) : QObject(parent)
{
    try
    {
        initLogging();
        initMarketService();
        initAccountService();
        readConfiguration();
        runCore();
    }
    catch (const std::exception& e)
    {
        coreCrit() << e.what();
        throw;
    };
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

void AppCore::runCore()
{   
    runMarketService();
    runAccountService();
}

void AppCore::initLogging()
{
    Core::Tools::LogManager::instance().setDebugEnabled(false);
    Core::Tools::LogManager::instance().setStdLogEnabled(false);
    Core::Tools::LogManager::instance().setLogFileEnabled(true);
    Core::Tools::LogManager::instance().setLogFile("app.log");
    m_logModel = std::make_unique<Core::Tools::LogModel>();
}

void AppCore::initMarketService()
{
    m_marketState = std::make_shared<Core::MarketDataState>();
    m_marketMediator = std::make_shared<Core::MarketDataMediator>();
    m_marketService = std::make_shared<Core::MarketDataService>(m_marketState, m_marketMediator);

    connect(m_marketService.get(), &Core::MarketDataService::messageReceived,
            this, [](const QString& m) { marketInfo().noquote() << m; });
    connect(m_marketService.get(), &Core::MarketDataService::warningOccurred,
            this, [](const QString& w) { marketWarn().noquote() << w; });
    connect(m_marketService.get(), &Core::MarketDataService::errorOccurred,
            this, [](const QString& e) { marketCrit().noquote() << e; });
}

void AppCore::runMarketService()
{
    m_marketMediator->init(m_config.testnet);

    m_marketMediator->loadInstruments(EnumHelpers::MarketType::Spot);
    m_marketMediator->loadInstruments(EnumHelpers::MarketType::Linear);

    if (m_config.autoConnection)
    {
        m_marketMediator->runAllStreams();
    }
}

void AppCore::initAccountService()
{
    m_accountMediator = std::make_shared<Core::AccountMediator>();
    m_accountState = std::make_shared<Core::AccountState>();
    m_accountService = std::make_shared<Core::AccountService>(m_accountState, m_accountMediator);

    connect(m_accountService.get(), &Core::AccountService::messageReceived,
            this, [](const QString& m) { accountInfo().noquote() << m; });
    connect(m_accountService.get(), &Core::AccountService::warningOccurred,
            this, [](const QString& w) { accountWarn().noquote() << w; });
    connect(m_accountService.get(), &Core::AccountService::errorOccurred,
            this, [](const QString& e) { accountCrit().noquote() << e; });
}

void AppCore::runAccountService()
{
    m_accountMediator->init(m_config.apis[m_config.activeApi], m_config.testnet);

    m_accountService->setEnableTrades(m_config.enableTrades);
    m_accountMediator->requestInfoAboutApi();
    m_accountMediator->requestWalletBalance();

    if (m_config.autoConnection)
    {
        m_accountMediator->runStreamer();
    }
}

void AppCore::readConfiguration()
{
    coreInfo() << "---------- Reading Configuration ----------";

    auto& configManager = Core::ConfigurationManager::instance();
    coreInfo() << "Config file:" << configManager.configPath();

    auto res = configManager.tryLoad();
    if (!res.has_value())
    {
        qCritical(lcCore) << res.error();
        return;
    }

    m_config = res.value();
    if (!m_config.apis.contains(m_config.activeApi))
    {
        coreCrit() << "Active API not found:" << m_config.activeApi;
        return;
    }

    coreInfo() << "Network:" << (m_config.testnet ? "testnet" : "mainnet");
    coreInfo() << "Trading:" << (m_config.enableTrades ? "enable" : "disable");

    if (!m_config.enableTrades)
        coreWarn().noquote() << "Trading was disabled";

    coreInfo() << "-------------------------------------------";
}