#ifndef GETBYBITFEERATEHANDLER_HPP
#define GETBYBITFEERATEHANDLER_HPP
#include "Service/IResponseHandler.hpp"
#include "Tools/RequestStructures.hpp"

namespace Core::Handlers
{

class GetBybitFeeRateHandler : public Service::IPrivateResponseHandler
{
public:
    static QString endpoint() { return "/v5/account/fee-rate"; }

    static QUrlQuery buildRequest(const Tools::FeeRateRequest& req);

    void handle(const QJsonObject& data, const QVariant& requestContext, Service::PrivateServiceSignals* signaller) override;
};

}

#endif // GETBYBITFEERATEHANDLER_HPP
