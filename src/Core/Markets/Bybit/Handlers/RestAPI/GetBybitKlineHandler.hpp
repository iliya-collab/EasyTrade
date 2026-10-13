#pragma once
#include "Markets/RestAPI/IResponseHandler.hpp"
#include "Tools/QueryStructures.hpp"

namespace Core::Markets
{

    class GetBybitKlineHandler : public IMarketDataResponseHandler
    {
    private:

        void processRequestKlines(QList<Tools::Kline>& klines, Tools::EnumHelpers::Interval interval, const QJsonObject& data);

    public:

        static QString endpoint() { return "/v5/market/kline"; }

        static QUrlQuery buildRequest(const Tools::KlinesRequest& req);

        void handle(const QJsonObject& data, const QVariant& requestContext, MarketDataServiceSignals* signaller) override;

    };
}