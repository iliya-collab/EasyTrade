#pragma once
#include "IMarketDataService.hpp"
#include "BaseRestService.hpp"

namespace Core::Service
{

class BaseMarketDataService : public BaseRestService<MarketDataServiceSignals, IMarketDataService>
{
    Q_OBJECT
public:

    explicit BaseMarketDataService(std::unique_ptr<Tools::BaseRestAPI> api, QObject* parent = nullptr)
        : BaseRestService(std::move(api), parent)
    {
        connect(signaller(), &MarketDataServiceSignals::instrumentsInfoPageReceived,
                this, &BaseMarketDataService::onInstrumentsInfoPageReceived);
    }

    virtual ~BaseMarketDataService() = default;

protected slots:

    virtual void onInstrumentsInfoPageReceived(const Core::Tools::Instruments&, const QString&, const QVariant&) = 0;

};

}

