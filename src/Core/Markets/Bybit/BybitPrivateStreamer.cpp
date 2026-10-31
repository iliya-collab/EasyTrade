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
          Tools::BybitWebSocketCreator::create(Tools::SocketType::Private, Tools::EnumHelpers::MarketType::Unknown, Tools::Api(), parent),
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

    void BybitPrivateStreamer::init(const Tools::Api &api, bool isTestnet)
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