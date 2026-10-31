#ifndef BASESTREAMER_HPP
#define BASESTREAMER_HPP
#include "Tools/Network/BaseWebSocket.hpp"
#include "IStreamHandler.hpp"
#include "StreamerConcepts.hpp"
#include "Tools/Logging.hpp"
#include <QObject>
#include <QSet>
#include <map>
#include <memory>

namespace Core::Streamer
{

template<typename TSignals, typename TInterface>
class BaseStreamer : public QObject, public TInterface
{
public:

    using Handler = IStreamHandler<TSignals>;

    explicit BaseStreamer(std::unique_ptr<Tools::BaseWebSocket> ws, QObject* parent = nullptr)
        : QObject(parent)
        , m_signaller(std::make_unique<TSignals>(this))
        , m_webSocket(std::move(ws))
    {
        if (!m_webSocket)
            return;
        connect(m_webSocket.get(), &Tools::BaseWebSocket::connected,       this, [this] { onStarted(); });
        connect(m_webSocket.get(), &Tools::BaseWebSocket::disconnected,    this, [this] { onStopped(); });
        connect(m_webSocket.get(), &Tools::BaseWebSocket::pingMeasured,    this, [this](qint64 ms) { onPingMeasured(ms); });
        connect(m_webSocket.get(), &Tools::BaseWebSocket::errorOccurred,   this, [this](const QString& e) { onErrorOccurred(e); });
        connect(m_webSocket.get(), &Tools::BaseWebSocket::messageReceived, this, [this](const QJsonObject& m) { onMessageReceived(m); });
    }

    ~BaseStreamer() override
    {
        if (!m_webSocket)
            return;
        QObject::disconnect(m_webSocket.get(), nullptr, this, nullptr);
        m_webSocket->close();
    }

    TSignals* signaller() const override { return m_signaller.get(); }

    void start() override
    {
        if (m_webSocket)
            m_webSocket->open();
    }

    bool isRunning() override { return m_webSocket && m_webSocket->isOpen(); }

    void stop() override
    {
        if (!m_webSocket)
            return;
        m_webSocket->close();
    }

    void restart() override
    {
        if (!m_webSocket)
            return;
        m_webSocket->reconnect();
    }

    void connectToStreams() override
    {
        if (!m_usedStreams.isEmpty())
            this->sendSubscriptionMessage(m_usedStreams.values());
    }

    void disconnectFromStreams() override
    {
        if (!m_usedStreams.isEmpty())
        {
            this->sendUnsubscriptionMessage(m_usedStreams.values());
            m_usedStreams.clear();
        }
    }

protected:

    virtual void onStarted()
    {
        connectToStreams();
        emit m_signaller->started(this->id());
    }
    virtual void onStopped()                       { emit m_signaller->stopped(this->id()); }
    virtual void onPingMeasured(qint64 ms)         { emit m_signaller->pingMeasured(this->id(), ms); }
    virtual void onErrorOccurred(const QString& e) { emit m_signaller->errorOccurred(this->id(), e); }

    virtual void onMessageReceived(const QJsonObject& msg)
    {
        const QString topic = msg.value("topic").toString();

        if (topic.isEmpty())
            return;

        for (auto& [prefix, h] : m_handlers)
        {
            if (topic.startsWith(prefix))
            {
                h->handle(this->id(), msg, m_signaller.get());
                return;
            }
        }
        emit m_signaller->errorOccurred(this->id(), "Unknown topic: " + topic);
    }

    template<typename H>
        requires HasTopic<H>
    void registerHandler()
    {
        static_assert(std::is_base_of_v<Handler, H>, "H must inherit from IStreamHandler<TSignals>");
        m_handlers[H::topic()] = std::make_unique<H>();
    }

    void addStreams(const QStringList& names)
    {
        QStringList fresh;
        for (const auto& n : names)
        {
            if (!n.isEmpty() && !m_usedStreams.contains(n))
            {
                m_usedStreams.insert(n);
                fresh << n;
            }
        }
        if (m_webSocket && m_webSocket->isOpen() && !fresh.isEmpty())
            this->sendSubscriptionMessage(fresh);
    }

    void removeStreams(const QStringList& names)
    {
        QStringList gone;
        for (const auto& n : names)
            if (m_usedStreams.remove(n))
                gone << n;
        if (m_webSocket && m_webSocket->isOpen() && !gone.isEmpty())
            this->sendUnsubscriptionMessage(gone);
    }

    std::unique_ptr<TSignals> m_signaller;
    std::unique_ptr<Tools::BaseWebSocket> m_webSocket;
    QSet<QString> m_usedStreams;
    std::map<QString, std::unique_ptr<Handler>> m_handlers;
};

}

#endif // BASESTREAMER_HPP
