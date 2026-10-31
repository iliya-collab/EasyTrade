#pragma once
#include "Service/BaseMarketDataService.hpp"

namespace Core
{

class BybitMarketDataService : public Service::BaseMarketDataService
{
    Q_OBJECT
public:

    explicit BybitMarketDataService(QObject* parent = nullptr);

    void init(bool isTestnet) override;

    void requestInstruments(const Tools::InstrumentsInfoRequest& req) override;
    void requestKlines(const Tools::KlinesRequest& req) override;

protected:

    void onInstrumentsInfoPageReceived(const Core::Tools::Instruments& instruments, const QString& nextPageCursor, const QVariant& context) override;


};

}

