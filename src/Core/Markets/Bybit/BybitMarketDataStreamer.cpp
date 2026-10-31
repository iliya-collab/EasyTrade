#include "BybitMarketDataStreamer.hpp"
#include "Tools/Network/Bybit/BybitWebSocketCreator.hpp"
#include "BybitWsProtocol.hpp"
#include "Handlers/Streamer/BybitTickerStreamHandler.hpp"
#include "Handlers/Streamer/BybitOrderbookStreamHandler.hpp"
#include "Handlers/Streamer/BybitKlineStreamHandler.hpp"
#include "Handlers/Streamer/BybitPublicTradeStreamHandler.hpp"
#include <QDateTime>

namespace Core
{

    BybitMarketDataStreamer::BybitMarketDataStreamer(Tools::EnumHelpers::MarketType type, QObject* parent)
        : BaseMarketDataStreamer(
            Tools::BybitWebSocketCreator::create(Tools::SocketType::Public, type, Tools::Api(), parent),
            parent
        )
    {
        qDebug() << Q_FUNC_INFO << "called from:" << QThread::currentThread();

        registerHandler<Handlers::BybitTickerStreamHandler>();
        registerHandler<Handlers::BybitOrderbookStreamHandler>();
        registerHandler<Handlers::BybitKlineStreamHandler>();
        registerHandler<Handlers::BybitPublicTradeStreamHandler>();
    }

    BybitMarketDataStreamer::~BybitMarketDataStreamer()
    {
        if (!m_webSocket)
            return;

        m_webSocket->close();
    }

    QString BybitMarketDataStreamer::id()
    {
        return m_webSocket->getId();
    }

    void BybitMarketDataStreamer::init(bool isTestnet)
    {
        Tools::Api api {"", ""};
        m_webSocket->init(api, isTestnet);
    }

    void BybitMarketDataStreamer::sendSubscriptionMessage(const QStringList &streams)
    {
        Bybit::WsProtocol::sendChunked(*m_webSocket, "subscribe", streams);
    }

    void BybitMarketDataStreamer::sendUnsubscriptionMessage(const QStringList &streams)
    {
        Bybit::WsProtocol::sendChunked(*m_webSocket, "unsubscribe", streams);
    }

    QString BybitMarketDataStreamer::createTickerStream(const QString &symbol) const
    {
        return QString("tickers.%1").arg(symbol);
    }

    QString BybitMarketDataStreamer::createOrderbookStream(const QString &symbol, int depth) const
    {
        return QString("orderbook.%1.%2")
            .arg(depth)
            .arg(symbol);
    }

    QString BybitMarketDataStreamer::createKlineStream(const QString &symbol, Tools::EnumHelpers::Interval interval) const
    {
        return QString("kline.%1.%2")
            .arg(Tools::EnumHelpers::intervalToString(interval))
            .arg(symbol);
    }

    QString BybitMarketDataStreamer::createPublicTradeStream(const QString &symbol) const
    {
        return QString("publicTrade.%1").arg(symbol);
    }

}