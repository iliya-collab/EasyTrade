#pragma once
#include <Tools/Network/BaseRestAPI.hpp>
#include "IResponseHandler.hpp"
#include "IMarketDataService.hpp"
#include "ServiceConcepts.hpp"
#include <QObject>

namespace Core::Markets
{

    class BaseMarketDataService : public IMarketDataService
    {
        Q_OBJECT
\
    private:

        std::map<QString, std::unique_ptr<IMarketDataResponseHandler>> m_handlers;
        MarketDataServiceSignals* m_signaller;

    protected:

        const int LOADING_TIMEOUT = 30000;
        std::unique_ptr<Tools::BaseRestAPI> m_currentApi;

        // Метод для регистрации обработчика endpoint
        template<typename IHandler>
            requires HasEndpoint<IHandler>
        void registerHandler()
        {
            static_assert(std::is_base_of<IMarketDataResponseHandler, IHandler>::value, "IHandler must inherit from IMarketDataResponseHandler!");

            auto response = std::make_unique<IHandler>();

            if (!response)
                return;

            QString endpoint = IHandler::endpoint();
            m_handlers[endpoint] = std::move(response);
        }

        // Метод для запроса
        template<typename IHandler, typename... Args>
            requires HasEndpoint<IHandler> && IsGetRequest<IHandler, Args...>
        void requestGetImpl(const QVariant& context, Args&&... args)
        {
            if (!m_currentApi)
                return;
            auto params = IHandler::buildRequest(std::forward<Args>(args)...);
            m_currentApi->requestEndpointGet(IHandler::endpoint(), params, LOADING_TIMEOUT, context);
        }

    public:

        explicit BaseMarketDataService(std::unique_ptr<Tools::BaseRestAPI> api, QObject* parent = nullptr)
            : IMarketDataService(parent), m_currentApi(std::move(api))
        {
            m_signaller = new MarketDataServiceSignals(this);

            connect(m_currentApi.get(), &Tools::BaseRestAPI::downloadProgress, m_signaller, &MarketDataServiceSignals::downloadProgress, Qt::UniqueConnection);
            connect(m_currentApi.get(), &Tools::BaseRestAPI::errorOccurred, m_signaller, &MarketDataServiceSignals::errorOccurred, Qt::UniqueConnection);
            connect(m_currentApi.get(), &Tools::BaseRestAPI::dataReceived, this, &BaseMarketDataService::onDataReceived, Qt::UniqueConnection);
        }

        virtual ~BaseMarketDataService() {}

        MarketDataServiceSignals* signaller() override { return m_signaller; }

    protected slots:

        virtual void onDataReceived(const QUrl& reqUrl, const QByteArray& data, const QVariant& context)
        {
            if (m_handlers.empty())
                return;

            QString path = reqUrl.path();

            if (m_handlers.contains(path))
                m_handlers[path]->handle(QJsonDocument::fromJson(data).object(), context, m_signaller);
            else
                emit m_signaller->errorOccurred("Unknown endpoint: " + path);
        }

    };

}

