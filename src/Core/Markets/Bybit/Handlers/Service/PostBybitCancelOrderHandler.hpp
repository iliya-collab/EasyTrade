#pragma once
#include "Service/IResponseHandler.hpp"
#include "Tools/RequestStructures.hpp"

namespace Core::Handlers
{

// POST запрос на отмену ордера
class PostBybitCancelOrderHandler : public Service::IPrivateResponseHandler
{
public:

    static QString endpoint() { return "/v5/order/cancel"; }

    static QByteArray buildRequestBody(const Tools::OrderCancelRequest& req);

    void handle(const QJsonObject& data, const QVariant& requestContext, Service::PrivateServiceSignals* signaller) override;

};

}
