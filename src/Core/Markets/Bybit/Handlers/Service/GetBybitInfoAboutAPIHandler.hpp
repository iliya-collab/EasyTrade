#pragma once
#include "Service/IResponseHandler.hpp"

namespace Core::Handlers
{

class GetBybitInfoAboutAPIHandler : public Service::IPrivateResponseHandler
{
private:

    QString parseISOFormat(const QString& time);

public:

    static QString endpoint() { return "/v5/user/query-api"; }

    static QUrlQuery buildRequest() { return QUrlQuery(); }

    void handle(const QJsonObject& data, const QVariant& requestContext, Service::PrivateServiceSignals* signaller) override;
};

}


