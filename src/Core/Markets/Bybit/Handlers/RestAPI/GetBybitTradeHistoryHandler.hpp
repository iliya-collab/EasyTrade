#ifndef GETBYBITTRADEHISTORYHANDLER_HPP
#define GETBYBITTRADEHISTORYHANDLER_HPP
#include "Markets/RestAPI/IResponseHandler.hpp"
#include "Tools/QueryStructures.hpp"

namespace Core::Markets
{

// GET запрос для получения записей о выполнении операций
class GetBybitTradeHistoryHandler : public IPrivateResponseHandler
{
public:

    static QString endpoint() { return "/v5/execution/list"; }

    static QUrlQuery buildRequest(const Tools::TradeHistoryRequest& req);

    void handle(const QJsonObject& data, const QVariant& requestContext, PrivateServiceSignals* signaller) override;
};

}

#endif // GETBYBITTRADEHISTORYHANDLER_HPP
