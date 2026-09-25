#include "GetBybitKlineHandler.hpp"

namespace Core::Markets
{
    void GetBybitKlineHandler::handle(const QJsonObject &data, const QVariant& requestContext, IMarketDataService *service)
    {
        if (!data.contains("retMsg") || data["retMsg"].toString() != "OK")
        {
            emit service->errorOccurred(QString("Error processing request [endpoint = %1]: " + data["retMsg"].toString()).arg(endpoint()));
            return;
        }

        if (!requestContext.canConvert<Tools::KlinesRequest>())
        {
            emit service->errorOccurred(QString("Missing request context [endpoint = %1]").arg(endpoint()));
            return;
        }

        Tools::KlinesRequest req = requestContext.value<Tools::KlinesRequest>();

        QList<Tools::Kline> klines;
        processRequestKlines(klines, req.m_interval, data);
        emit service->klinesReceived(klines);
    }

    void GetBybitKlineHandler::processRequestKlines(QList<Tools::Kline>& klines, Tools::Interval interval, const QJsonObject& data)
    {
        QJsonObject result = data["result"].toObject();
        QString symbol = result["symbol"].toString();
        QString category = result["category"].toString();
        QJsonArray list = result["list"].toArray();

        qint64 durationMs = Tools::intervalToMs(interval);

        for (const auto& obj : std::as_const(list))
        {
            QJsonArray itemArr = obj.toArray();
            Tools::Kline kline;
            kline.m_category = Tools::stringToMarketType(category);
            kline.m_symbol = symbol;
            kline.m_start = itemArr[0].toString().toLongLong();
            kline.m_end = kline.m_start + durationMs;
            kline.m_open = itemArr[1].toString();
            kline.m_high = itemArr[2].toString();
            kline.m_low = itemArr[3].toString();
            kline.m_close = itemArr[4].toString();
            kline.m_volume = itemArr[5].toString();
            kline.m_turnover = itemArr[6].toString();
            kline.m_confirm = true;
            kline.m_interval = interval;
            klines.append(kline);
        }
    }

    QUrlQuery GetBybitKlineHandler::buildRequest(const Tools::KlinesRequest &req)
    {
        QUrlQuery params;
        params.addQueryItem("category", Tools::marketTypeToString(req.m_category));
        params.addQueryItem("symbol", req.m_symbol);
        params.addQueryItem("interval", Tools::intervalToString(req.m_interval));
        if (req.m_start.has_value())
            params.addQueryItem("start", QString::number(req.m_start.value()));
        if (req.m_end.has_value())
            params.addQueryItem("end", QString::number(req.m_end.value()));
        if (req.m_limit.has_value())
            params.addQueryItem("limit", QString::number(req.m_limit.value()));
        return params;
    }
}
