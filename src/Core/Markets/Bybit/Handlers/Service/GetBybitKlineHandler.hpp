#pragma once
#include "Service/IResponseHandler.hpp"
#include "Tools/RequestStructures.hpp"

namespace Core::Handlers
{

class GetBybitKlineHandler : public Service::IMarketDataResponseHandler
{
private:

    void processRequestKlines(QList<Tools::Kline>& klines,
                              Tools::EnumHelpers::Interval interval,
                              const QJsonObject& data);

public:

    static QString endpoint() { return "/v5/market/kline"; }

    static QUrlQuery buildRequest(const Tools::KlinesRequest& req);

    void handle(const QJsonObject& data,
                const QVariant& requestContext,
                Service::MarketDataServiceSignals* signaller) override;

};

}