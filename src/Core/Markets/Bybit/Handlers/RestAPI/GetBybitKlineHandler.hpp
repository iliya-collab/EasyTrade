#pragma once
#include "Markets/RestAPI/IResponseHandler.hpp"

namespace Core::Markets
{

    class GetBybitKlineHandler : public IMarketDataResponseHandler
    {
    private:

        void processRequestKlines(QList<Tools::Kline>& klines, const QString& interval, const QJsonObject& data);

    public:

        static QString endpoint() { return "/v5/market/kline"; }

        static QUrlQuery buildRequest(const Tools::KlinesRequest& req)
        {
            QUrlQuery params;
            params.addQueryItem("category", Tools::marketTypeToString(req.m_category));
            params.addQueryItem("symbol", req.m_symbol);
            params.addQueryItem("interval", Tools::intervalToString(req.m_interval));
            params.addQueryItem("start", QString::number(req.m_start.value()));
            params.addQueryItem("end", QString::number(req.m_end.value()));
            params.addQueryItem("limit", QString::number(req.m_limit.value()));
            return params;
        }

        void handle(const QJsonObject& data, IMarketDataService* service) override;

    };
}