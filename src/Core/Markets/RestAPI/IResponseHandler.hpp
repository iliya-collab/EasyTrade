#pragma once
#include "MarketDataServiceSignals.hpp"
#include "PrivateServiceSignals.hpp"

namespace Core::Markets
{

template <typename TServiceSignaller>
class IResponseHandler
{
public:
    virtual ~IResponseHandler() = default;
    // Метод, который принимает сырой JSON и выполняет парсинг
    virtual void handle(const QJsonObject& data, const QVariant& requestContext, TServiceSignaller* signaller) = 0;
};

using IMarketDataResponseHandler = IResponseHandler<MarketDataServiceSignals>;
using IPrivateResponseHandler = IResponseHandler<PrivateServiceSignals>;

}
