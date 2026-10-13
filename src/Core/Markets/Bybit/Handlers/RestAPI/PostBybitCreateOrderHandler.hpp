#pragma once
#include "Markets/RestAPI/IResponseHandler.hpp"
#include "Tools/QueryStructures.hpp"

namespace Core::Markets
{

// POST запрос на создания ордера
class PostBybitCreateOrderHandler : public IPrivateResponseHandler
{
public:

    static QString endpoint() { return "/v5/order/create"; }

    static QByteArray buildRequestBody(const Tools::OrderRequest& req);

    void handle(const QJsonObject& data, const QVariant& requestContext, PrivateServiceSignals* signaller) override;

};

}