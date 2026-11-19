#include "BybitMarketDataService.hpp"
#include "Tools/Network/Bybit/BybitRestAPI.hpp"
#include "Tools/PagedJob.hpp"
#include "Handlers/Service/GetBybitInstrumentsInfoHandler.hpp"
#include "Handlers/Service/GetBybitKlineHandler.hpp"

using InstrumentsInfoJob = Core::Tools::PagedJob<InstrumentsInfoRequest, InstrumentInfo>;

namespace Core
{

BybitMarketDataService::BybitMarketDataService(QObject* parent)
    : BaseMarketDataService(std::make_unique<Tools::BybitRestAPI>(parent), parent)
{
    registerHandler<Handlers::GetBybitInstrumentsInfoHandler>();
    registerHandler<Handlers::GetBybitKlineHandler>();
}

void BybitMarketDataService::init(bool isTestnet)
{
    if (!m_currentApi)
        return;

    Api api {"", ""};
    m_currentApi->init(api, isTestnet);
}

void BybitMarketDataService::requestInstruments(const Tools::InstrumentsInfoRequest& req)
{
    InstrumentsInfoJob job;
    job.setRequest(req);
    send<Handlers::GetBybitInstrumentsInfoHandler>(QVariant::fromValue(job), job.getRequest());
}

void BybitMarketDataService::requestKlines(const Tools::KlinesRequest& req)
{
    send<Handlers::GetBybitKlineHandler>(QVariant::fromValue(req), req);
}

void BybitMarketDataService::onInstrumentsInfoPageReceived(const Instruments &instruments, const QString &nextPageCursor, const QVariant &context)
{
    if (context.metaType() != QMetaType::fromType<InstrumentsInfoJob>())
    {
        emit signaller()->instrumentsInfoPageReceived(instruments, nextPageCursor, context);
        return;
    }

    auto job = context.value<InstrumentsInfoJob>();
    if (job.addPage(instruments, nextPageCursor))
        send<Handlers::GetBybitInstrumentsInfoHandler>(QVariant::fromValue(job), job.getRequest());
    else
        emit signaller()->allInstrumentsInfoReceived(job.get(), job.wasTruncated());
}

}

