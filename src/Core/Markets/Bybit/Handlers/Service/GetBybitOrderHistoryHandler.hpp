#pragma once
#include "Service/IResponseHandler.hpp"
#include "Tools/RequestStructures.hpp"

namespace Core::Handlers
{

// GET запрос на получения истории ордеров
class GetBybitOrderHistoryHandler : public Service::IPrivateResponseHandler
{
public:

    static QString endpoint() { return "/v5/order/history"; }

    static QUrlQuery buildRequest(const Tools::OrderHistoryRequest& req);

    void handle(const QJsonObject& data, const QVariant& requestContext, Service::PrivateServiceSignals* signaller) override;
};

}
