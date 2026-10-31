#pragma once
#include "Service/IResponseHandler.hpp"
#include "Tools/RequestStructures.hpp"

namespace Core::Handlers
{

// POST запрос на создания ордера
class PostBybitCreateOrderHandler : public Service::IPrivateResponseHandler
{
public:

    static QString endpoint() { return "/v5/order/create"; }

    static QByteArray buildRequestBody(const Tools::OrderRequest& req);

    void handle(const QJsonObject& data, const QVariant& requestContext, Service::PrivateServiceSignals* signaller) override;

};

}