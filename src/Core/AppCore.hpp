#pragma once
#include "MarketData/MarketDataService.hpp"
#include "AccountData/AccountService.hpp"
#include "Config/Configuration.hpp"
#include "Tools/DataModels/LogModel.hpp"
#include <QQmlContext>
#include <QObject>
#include <memory>

class AppCore : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(Core::MarketDataState*   marketState     READ getMarketState     CONSTANT)
    Q_PROPERTY(Core::MarketDataService* marketService   READ getMarketService   CONSTANT)
    Q_PROPERTY(Core::AccountState*      accountState    READ getAccountState    CONSTANT)
    Q_PROPERTY(Core::AccountService*    accountService  READ getAccountService  CONSTANT)
    Q_PROPERTY(Core::Tools::LogModel*   logModel        READ getLogModel        CONSTANT)

private:

    std::shared_ptr<Core::MarketDataMediator> m_marketMediator;
    std::shared_ptr<Core::MarketDataState> m_marketState;
    std::shared_ptr<Core::MarketDataService> m_marketService;

    std::shared_ptr<Core::AccountMediator> m_accountMediator;
    std::shared_ptr<Core::AccountState> m_accountState;
    std::shared_ptr<Core::AccountService> m_accountService;

    std::unique_ptr<Core::Tools::LogModel> m_logModel;

    Core::ConfigurationParams m_config{};

    QThread* m_marketThread = nullptr;
    QThread* m_accountThread = nullptr;

    void initCore();
    void runCore();

    void initLogging();

    void initMarketService();
    void runMarketService();

    void initAccountService();
    void runAccountService();

    void readConfiguration();

    void shutdownThreads();

    template<class T, class... Args>
    std::shared_ptr<T> makeInThread(QThread* th, Args&&... args)
    {
        auto* ctx = new QObject;
        ctx->moveToThread(th);

        T* raw = nullptr;
        QMetaObject::invokeMethod(ctx, [&] {
            raw = new T(std::forward<Args>(args)...);
        }, Qt::BlockingQueuedConnection);

        ctx->deleteLater();

        QObject::connect(th, &QThread::finished, raw, &QObject::deleteLater);
        return std::shared_ptr<T>(raw, [](T*) {});
    }

    template<class T, class F>
    void postTo(T* obj, F&& f)
    {
        QMetaObject::invokeMethod(obj, std::forward<F>(f));
    }

    explicit AppCore(QObject* parent = nullptr);
    ~AppCore() = default;

public:

    static AppCore& instance();
    // --- Singleton для QML ---
    static AppCore* create(QQmlEngine*, QJSEngine*);

    Core::MarketDataState* getMarketState() const { return m_marketState.get(); }
    Core::MarketDataService* getMarketService() const { return m_marketService.get(); }
    Core::AccountState* getAccountState() const { return m_accountState.get(); }
    Core::AccountService* getAccountService() const { return m_accountService.get(); }
    Core::Tools::LogModel* getLogModel() const { return m_logModel.get(); }

};