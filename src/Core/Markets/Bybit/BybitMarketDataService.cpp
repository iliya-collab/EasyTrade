#include "BybitMarketDataService.hpp"
#include "Tools/Network/Bybit/BybitRestAPI.hpp"
#include "Handlers/RestAPI/GetBybitTradePairsHandler.hpp"
#include "Handlers/RestAPI/GetBybitKlineHandler.hpp"

namespace Core::Markets
{

    BybitMarketDataService::BybitMarketDataService(QObject* parent)
        : BaseMarketDataService(std::make_unique<Tools::BybitRestAPI>(parent), parent)
    {
        registerHandler<GetBybitTradePairsHandler>();
        registerHandler<GetBybitKlineHandler>();
    }

    void BybitMarketDataService::init(bool isTestnet)
    {
        qDebug() << Q_FUNC_INFO << "called from:" << QThread::currentThread();

        if (!m_currentApi)
            return;

        Tools::Api api {"", "", isTestnet};
        m_currentApi->init(api);
    }

    void BybitMarketDataService::requestTradePairs(Tools::MarketType type)
    {
        qDebug() << Q_FUNC_INFO << "called from:" << QThread::currentThread();

        requestGetImpl<GetBybitTradePairsHandler>(type);
    }

    void BybitMarketDataService::requestKlines(const Tools::KlinesRequest& req)
    {
        qDebug() << Q_FUNC_INFO << "called from:" << QThread::currentThread();

        requestGetImpl<GetBybitKlineHandler>(req);
    }

}

