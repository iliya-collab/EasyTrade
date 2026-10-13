#pragma once
#include "Markets/RestAPI/IResponseHandler.hpp"
#include "Tools/QueryStructures.hpp"

namespace Core::Markets
{

// POST запрос на отмену ордера
class PostBybitCancelOrderHandler : public IPrivateResponseHandler
{
public:

    static QString endpoint() { return "/v5/order/cancel"; }

    static QByteArray buildRequestBody(const Tools::OrderCancelRequest& req);

    void handle(const QJsonObject& data, const QVariant& requestContext, PrivateServiceSignals* signaller) override;

};

}
