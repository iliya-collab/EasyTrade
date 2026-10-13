#pragma once
#include "Markets/RestAPI/IResponseHandler.hpp"
#include "Tools/QueryStructures.hpp"

namespace Core::Markets
{

// POST запрос на отмену всех ордеров
class PostBybitCancelAllOrdersHandler : public IPrivateResponseHandler
{
public:

    static QString endpoint() { return "/v5/order/cancel-all"; }

    static QByteArray buildRequestBody(const Tools::OrderCancelAllRequest& req);

    void handle(const QJsonObject& data, const QVariant& requestContext, PrivateServiceSignals* signaller) override;

};

}
