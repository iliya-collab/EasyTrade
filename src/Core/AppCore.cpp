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
        initCore();
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

void AppCore::initCore()
{
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit,
            this, &AppCore::shutdownThreads);

    initLogging();
    initMarketService();
    initAccountService();
}

void AppCore::runCore()
{
    readConfiguration();
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
    m_marketThread = new QThread(this);
    m_marketThread->setObjectName("market-worker");
    m_marketThread->start();

    m_marketMediator = makeInThread<Core::MarketDataMediator>(m_marketThread);
    m_marketState = std::make_shared<Core::MarketDataState>(this);
    m_marketService = std::make_shared<Core::MarketDataService>(m_marketState, m_marketMediator, this);

    connect(m_marketService.get(), &Core::MarketDataService::messageReceived,
            this, [](const QString& m) { marketInfo().noquote() << m; });
    connect(m_marketService.get(), &Core::MarketDataService::warningOccurred,
            this, [](const QString& w) { marketWarn().noquote() << w; });
    connect(m_marketService.get(), &Core::MarketDataService::errorOccurred,
            this, [](const QString& e) { marketCrit().noquote() << e; });
}

void AppCore::runMarketService()
{
    postTo(m_marketMediator.get(), [m = m_marketMediator.get(), cfg = m_config] {
        m->init(cfg.testnet);
        m->loadInstruments(EnumHelpers::MarketType::Spot);
        m->loadInstruments(EnumHelpers::MarketType::Linear);
        if (cfg.autoConnection)
            m->runAllStreams();
    });
}

void AppCore::initAccountService()
{
    m_accountThread = new QThread(this);
    m_accountThread->setObjectName("account-worker");
    m_accountThread->start();

    m_accountMediator = makeInThread<Core::AccountMediator>(m_accountThread);
    m_accountState = std::make_shared<Core::AccountState>(this);
    m_accountService = std::make_shared<Core::AccountService>(m_accountState, m_accountMediator, this);

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

    postTo(m_accountMediator.get(), [m = m_accountMediator.get(), cfg = m_config] {
        if (!cfg.apis.contains(cfg.activeApi))
            return;

        m->init(cfg.apis[cfg.activeApi], cfg.testnet);
        m->requestInfoAboutApi();
        m->requestWalletBalance();
        if (cfg.autoConnection)
            m->runStreamer();
    });
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
        coreWarn() << "Active API not found:" << m_config.activeApi;

    coreInfo() << "Network:" << (m_config.testnet ? "testnet" : "mainnet");
    coreInfo() << "Trading:" << (m_config.enableTrades ? "enable" : "disable");

    if (!m_config.enableTrades)
        coreWarn().noquote() << "Trading was disabled";

    coreInfo() << "-------------------------------------------";
}

void AppCore::shutdownThreads()
{
    for (auto* th : {m_marketThread, m_accountThread}) {
        th->quit();
        th->wait();
    }
}