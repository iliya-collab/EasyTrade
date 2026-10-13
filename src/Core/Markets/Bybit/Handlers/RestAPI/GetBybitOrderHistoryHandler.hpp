#pragma once
#include "Markets/RestAPI/IResponseHandler.hpp"
#include "Tools/QueryStructures.hpp"

namespace Core::Markets
{

// GET запрос на получения истории ордеров
class GetBybitOrderHistoryHandler : public IPrivateResponseHandler
{
public:

    static QString endpoint() { return "/v5/order/history"; }

    static QUrlQuery buildRequest(const Tools::OrderHistoryRequest& req);

    void handle(const QJsonObject& data, const QVariant& requestContext, PrivateServiceSignals* signaller) override;
};

}
