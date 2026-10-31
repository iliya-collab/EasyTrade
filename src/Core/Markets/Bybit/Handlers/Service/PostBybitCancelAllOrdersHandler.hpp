#pragma once
#include "Service/IResponseHandler.hpp"
#include "Tools/RequestStructures.hpp"

namespace Core::Handlers
{

// POST запрос на отмену всех ордеров
class PostBybitCancelAllOrdersHandler : public Service::IPrivateResponseHandler
{
public:

    static QString endpoint() { return "/v5/order/cancel-all"; }

    static QByteArray buildRequestBody(const Tools::OrderCancelAllRequest& req);

    void handle(const QJsonObject& data, const QVariant& requestContext, Service::PrivateServiceSignals* signaller) override;

};

}
