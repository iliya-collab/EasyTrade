#pragma once
#include "Markets/RestAPI/IResponseHandler.hpp"
#include "Tools/StdTypes.hpp"

namespace Core::Markets
{

// GET запрос на получения "открытых" или "закрытых" ордеров
class GetBybitOpenOrdersHandler : public IPrivateResponseHandler
{
public:

    static QString endpoint() { return "/v5/order/realtime"; }

    static QUrlQuery buildRequest(const Tools::OpenOrdersRequest& req);

    void handle(const QJsonObject& data, IPrivateService* service) override;
};

}
