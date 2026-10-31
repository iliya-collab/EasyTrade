#ifndef GETBYBITTRADEHISTORYHANDLER_HPP
#define GETBYBITTRADEHISTORYHANDLER_HPP
#include "Service/IResponseHandler.hpp"
#include "Tools/RequestStructures.hpp"

namespace Core::Handlers
{

// GET запрос для получения записей о выполнении операций
class GetBybitTradeHistoryHandler : public Service::IPrivateResponseHandler
{
public:

    static QString endpoint() { return "/v5/execution/list"; }

    static QUrlQuery buildRequest(const Tools::TradeHistoryRequest& req);

    void handle(const QJsonObject& data, const QVariant& requestContext, Service::PrivateServiceSignals* signaller) override;
};

}

#endif // GETBYBITTRADEHISTORYHANDLER_HPP
