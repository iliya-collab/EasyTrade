#pragma once
#include "Service/IResponseHandler.hpp"

namespace Core::Handlers
{

class GetBybitAccountBalanceHandler : public Service::IPrivateResponseHandler
{
public:

    static QString endpoint() { return "/v5/account/wallet-balance"; }

    static QUrlQuery buildRequest()
    {
        QUrlQuery query;
        query.addQueryItem("accountType", "UNIFIED");
        return query;
    }

    void handle(const QJsonObject& data, const QVariant& requestContext, Service::PrivateServiceSignals* signaller) override;
};

}

