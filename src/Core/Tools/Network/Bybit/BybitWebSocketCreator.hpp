#pragma once
#include "BybitWebSocket.hpp"

namespace Core::Tools
{

    class BybitWebSocketCreator
    {
    public:

        static std::unique_ptr<BybitWebSocket> create(SocketType socketType, EnumHelpers::MarketType marketType, const Api& api, QObject* parent = nullptr)
        {
            auto newBybitWebSocket = std::make_unique<BybitWebSocket>(socketType, marketType, parent);
            newBybitWebSocket->setId(EnumHelpers::marketTypeToApiString(marketType));
            newBybitWebSocket->init(api);
            return newBybitWebSocket;
        }

    };

}