#pragma once
#include "Service/IResponseHandler.hpp"
#include "Tools/RequestStructures.hpp"

namespace Core::Handlers
{

class GetBybitInstrumentsInfoHandler : public Service::IMarketDataResponseHandler
{
private:

    void processRequestTradePairs(Tools::Instruments& instruments, const QJsonObject& result);

public:

    static QString endpoint() { return "/v5/market/instruments-info"; }

    static QUrlQuery buildRequest(const Tools::InstrumentsInfoRequest& req);

    void handle(const QJsonObject& data, const QVariant& requestContext, Service::MarketDataServiceSignals* signaller) override;
};

}

