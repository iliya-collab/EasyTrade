#ifndef BYBITWSPROTOCOL_HPP
#define BYBITWSPROTOCOL_HPP
#include "Tools/Network/BaseWebSocket.hpp"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDateTime>
#include <QThread>

namespace Core::Bybit::WsProtocol
{

inline constexpr int MAX_STREAMS_PER_MESSAGE = 10;

inline void sendChunked(Tools::BaseWebSocket& ws, const QString& op, const QStringList& streams)
{
    for (int i = 0; i < streams.size(); i += MAX_STREAMS_PER_MESSAGE)
    {
        QJsonObject msg;
        msg["op"] = op;
        msg["args"] = QJsonArray::fromStringList(streams.mid(i, MAX_STREAMS_PER_MESSAGE));
        msg["req_id"] = QString::number(QDateTime::currentMSecsSinceEpoch());
        ws.sendMessage(QJsonDocument(msg).toJson(QJsonDocument::Compact));

        if (i + MAX_STREAMS_PER_MESSAGE < streams.size())
            QThread::msleep(100);
    }
}

}

#endif // BYBITWSPROTOCOL_HPP
