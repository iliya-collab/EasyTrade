#include "BybitKlineStreamHandler.hpp"

void Core::Handlers::BybitKlineStreamHandler::handle(const QString& id, const QJsonObject &obj, Streamer::MarketDataStreamerSignals *signaller)
{
    if (!obj.contains("data") || !obj["data"].isArray())
    {
        emit signaller->errorOccurred(id, "[" + topic() + "] Invalid response structure!");
        return;
    }

    QJsonArray arrData = obj["data"].toArray();
    QString symbol = obj["topic"].toString().section('.', -1);

    Tools::Kline kline{};
    kline.m_symbol = symbol;

    for (const auto& val : std::as_const(arrData))
    {
        QJsonObject data = val.toObject();
        kline.m_category = Tools::EnumHelpers::apiStringToMarketType(id);
        kline.m_open = data["open"].toString();
        kline.m_close = data["close"].toString();
        kline.m_high = data["high"].toString();
        kline.m_low = data["low"].toString();
        kline.m_confirm = data["confirm"].toBool();
        kline.m_start = data["start"].toVariant().toLongLong();
        kline.m_end = data["end"].toVariant().toLongLong();
        kline.m_interval = Tools::EnumHelpers::stringToInterval(data["interval"].toString());
        kline.m_volume = data["volume"].toString();
        kline.m_turnover = data["turnover"].toString();
    }

    emit signaller->klineUpdated(kline);
}
