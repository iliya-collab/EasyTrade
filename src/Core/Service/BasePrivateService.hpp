#pragma once
#include "IPrivateService.hpp"
#include "BaseRestService.hpp"

namespace Core::Service
{

class BasePrivateService : public BaseRestService<PrivateServiceSignals, IPrivateService>
{
    Q_OBJECT

public:

    explicit BasePrivateService(std::unique_ptr<Tools::BaseRestAPI> api, QObject* parent = nullptr)
        : BaseRestService(std::move(api), parent)
    {
        connect(signaller(), &PrivateServiceSignals::orderHistoryPageReceived,
                this, &BasePrivateService::onOrderHistoryPageReceived);
        connect(signaller(), &PrivateServiceSignals::openOrdersPageReceived,
                this, &BasePrivateService::onOpenOrdersPageReceived);
        connect(signaller(), &PrivateServiceSignals::tradeHistoryPageReceived,
                this, &BasePrivateService::onTradeHistoryPageReceived);
    }

    virtual ~BasePrivateService() = default;

protected slots:

    virtual void onOrderHistoryPageReceived(const Orders&, const QString&, const QVariant&) = 0;
    virtual void onOpenOrdersPageReceived(const Orders&, const QString&, const QVariant&) = 0;
    virtual void onTradeHistoryPageReceived(const Executions&, const QString&, const QVariant&) = 0;

};

}

