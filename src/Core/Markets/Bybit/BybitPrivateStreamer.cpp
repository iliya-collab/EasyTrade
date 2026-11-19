#include "BybitPrivateStreamer.hpp"
#include "Tools/Network/Bybit/BybitWebSocketCreator.hpp"
#include "BybitWsProtocol.hpp"
#include "Handlers/Streamer/BybitWalletStreamHandler.hpp"
#include "Handlers/Streamer/BybitOrderStreamHandler.hpp"
#include "Handlers/Streamer/BybitExecutionStreamHandler.hpp"
#include "Handlers/Streamer/BybitPositionStreamHandler.hpp"
#include <QDateTime>

namespace Core
{

    BybitPrivateStreamer::BybitPrivateStreamer(QObject* parent)
        : BasePrivateStreamer(
          BybitWebSocketCreator::create(SocketType::Private, EnumHelpers::MarketType::Unknown, Api(), parent),
          parent
        )
    {
        registerHandler<Handlers::BybitWalletStreamHandler>();
        registerHandler<Handlers::BybitOrderStreamHandler>();
        registerHandler<Handlers::BybitExecutionStreamHandler>();
        registerHandler<Handlers::BybitPositionStreamHandler>();
    }

    BybitPrivateStreamer::~BybitPrivateStreamer()
    {
        if (!m_webSocket)
            return;

        m_webSocket->close();
    }

    QString BybitPrivateStreamer::id()
    {
        return "private";
    }

    EnumHelpers::MarketType BybitPrivateStreamer::marketType()
    {
        return Eh::MarketType::Unknown;
    }

    void BybitPrivateStreamer::init(const Api &api, bool isTestnet)
    {
        m_webSocket->init(api, isTestnet);
    }

    void BybitPrivateStreamer::sendSubscriptionMessage(const QStringList &streams)
    {
        Bybit::WsProtocol::sendChunked(*m_webSocket, "subscribe", streams);
    }

    void BybitPrivateStreamer::sendUnsubscriptionMessage(const QStringList &streams)
    {
        Bybit::WsProtocol::sendChunked(*m_webSocket, "unsubscribe", streams);
    }

    QString BybitPrivateStreamer::createWalletStream() const
    {
        return "wallet";
    }

    QString BybitPrivateStreamer::createOrderStream() const
    {
        return "order";
    }

    QString BybitPrivateStreamer::createPositionStream() const
    {
        return "position";
    }

    QString BybitPrivateStreamer::createExecutionStream() const
    {
        return "execution";
    }

}