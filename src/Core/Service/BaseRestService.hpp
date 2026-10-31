#ifndef BASERESTSERVICE_HPP
#define BASERESTSERVICE_HPP
#include "Tools/Network/BaseRestAPI.hpp"
#include "IResponseHandler.hpp"
#include "ServiceConcepts.hpp"
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <map>
#include <memory>

namespace Core::Service
{

// TSignals     = MarketDataServiceSignals  /   PrivateServiceSignals
// TInterface   = IMarketDataService        /   IPrivateService
// THandler     = IResponseHandler<TSignals>
template<typename TSignals, typename TInterface>
class BaseRestService : public QObject, public TInterface
{
public:

    using Handler = IResponseHandler<TSignals>;

    explicit BaseRestService(std::unique_ptr<Tools::BaseRestAPI> api, QObject* parent = nullptr)
        : QObject(parent)
        , m_signaller(std::make_unique<TSignals>(this))
        , m_currentApi(std::move(api))
    {
        connect(m_currentApi.get(), &Tools::BaseRestAPI::downloadProgress,
                m_signaller.get(), &ServiceSignals::downloadProgress, Qt::UniqueConnection);
        connect(m_currentApi.get(), &Tools::BaseRestAPI::errorOccurred,
                m_signaller.get(), &ServiceSignals::errorOccurred, Qt::UniqueConnection);
        connect(m_currentApi.get(), &Tools::BaseRestAPI::dataReceived,
                this, [this](const QUrl& url, const QByteArray& data, const QVariant& ctx) {
                    onDataReceived(url, data, ctx);
                });
    }

    ~BaseRestService() override = default;

    TSignals* signaller() const override { return m_signaller.get(); }

protected:

    static constexpr int LOADING_TIMEOUT = 30000;

    virtual void onDataReceived(const QUrl& reqUrl, const QByteArray& data, const QVariant& context)
    {
        const QString path = reqUrl.path();
        auto it = m_handlers.find(path);
        if (it == m_handlers.end()) {
            emit m_signaller->errorOccurred("Unknown endpoint: " + path);
            return;
        }
        it->second->handle(QJsonDocument::fromJson(data).object(), context, m_signaller.get());
    }

    template<typename H>
        requires HasEndpoint<H>
    void registerHandler()
    {
        static_assert(std::is_base_of_v<Handler, H>, "H must inherit from IResponseHandler<TSignals>");
        m_handlers[H::endpoint()] = std::make_unique<H>();
    }

    template<typename Endpoint, typename... Args>
    void send(const QVariant& context, Args&&... args)
    {
        static_assert(HasEndpoint<Endpoint>, "No endpoint()");
        static_assert(HasBuildAny<Endpoint, Args...>, "Handler has neither buildRequest nor buildRequestBody");
        static_assert(!HasBuildBoth<Endpoint, Args...>, "Handler must not have both buildRequest and buildRequestBody");

        if (!m_currentApi)
            return;

        if constexpr (IsGetRequest<Endpoint, Args...>)
            m_currentApi->requestEndpointGet(Endpoint::endpoint(),
                                             Endpoint::buildRequest(std::forward<Args>(args)...), LOADING_TIMEOUT, context);
        else if constexpr (IsPostRequest<Endpoint, Args...>)
            m_currentApi->requestEndpointPost(Endpoint::endpoint(),
                                              Endpoint::buildRequestBody(std::forward<Args>(args)...), LOADING_TIMEOUT, context);
        else
            static_assert(sizeof...(Args) < 0, "Endpoint doesn't match GET or POST signature");
    }

    std::unique_ptr<Tools::BaseRestAPI> m_currentApi;

private:

    std::map<QString, std::unique_ptr<Handler>> m_handlers;
    std::unique_ptr<TSignals> m_signaller;
};

}

#endif // BASERESTSERVICE_HPP
