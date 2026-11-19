#include "BybitTickerStreamHandler.hpp"

void Core::Handlers::BybitTickerStreamHandler::handle(const QString& id, const QJsonObject &obj, Streamer::MarketDataStreamerSignals *signaller)
{
    if (!obj.contains("data") || !obj["data"].isObject())
    {
        emit signaller->errorOccurred(id, "[" + topic() + "] Invalid response structure!");
        return;
    }

    QJsonObject data = obj["data"].toObject();
    QString symbol = data["symbol"].toString();

    TickerInfo ticker{};

    ticker.m_category = EnumHelpers::apiStringToMarketType(id);
    ticker.m_symbol = symbol;
    ticker.m_lastPrice = data["lastPrice"].toString();
    ticker.m_usdIndexPrice = data["usdIndexPrice"].toString();
    ticker.m_high24h = data["highPrice24h"].toString();
    ticker.m_low24h = data["lowPrice24h"].toString();
    ticker.m_volCcy24h = data["turnover24h"].toString();
    ticker.m_vol24h = data["volume24h"].toString();
    ticker.m_prevPrice24h = data["prevPrice24h"].toString();
    ticker.m_price24hPcnt = data["price24hPcnt"].toString();

    emit signaller->tickerUpdated(ticker);
}
