#pragma once
#include "Service/IResponseHandler.hpp"
#include "Tools/RequestStructures.hpp"

namespace Core::Handlers
{

// GET запрос на получения "открытых" или "закрытых" ордеров
class GetBybitOpenOrdersHandler : public Service::IPrivateResponseHandler
{
public:

    static QString endpoint() { return "/v5/order/realtime"; }

    static QUrlQuery buildRequest(const Tools::OpenOrdersRequest& req);

    void handle(const QJsonObject& data, const QVariant& requestContext, Service::PrivateServiceSignals* signaller) override;
};

}
