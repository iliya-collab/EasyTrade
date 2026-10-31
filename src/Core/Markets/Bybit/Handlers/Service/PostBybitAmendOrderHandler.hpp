#pragma once
#include "Service/IResponseHandler.hpp"
#include "Tools/RequestStructures.hpp"

namespace Core::Handlers
{

// POST запрос на исправление ордера
class PostBybitAmendOrderHandler : public Service::IPrivateResponseHandler
{
public:

    static QString endpoint() { return "/v5/order/amend"; }

    static QByteArray buildRequestBody(const Tools::OrderAmendRequest& req);

    void handle(const QJsonObject& data, const QVariant& requestContext, Service::PrivateServiceSignals* signaller) override;

};

}
