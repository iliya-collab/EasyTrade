#pragma once
#include "Markets/RestAPI/IResponseHandler.hpp"

namespace Core::Markets
{

    class GetBybitKlineHandler : public IMarketDataResponseHandler
    {
    private:

        void processRequestKlines(QList<Tools::Kline>& klines, Tools::Interval interval, const QJsonObject& data);

    public:

        static QString endpoint() { return "/v5/market/kline"; }

        static QUrlQuery buildRequest(const Tools::KlinesRequest& req);

        void handle(const QJsonObject& data, const QVariant& requestContext, IMarketDataService* service) override;

    };
}