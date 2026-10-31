#pragma once
#include "MarketDataServiceSignals.hpp"
#include "Tools/RequestStructures.hpp"

namespace Core::Service
{

class IMarketDataService
{
public:

    virtual ~IMarketDataService() = default;

    virtual void init(bool isTestnet) = 0;

    // Доступ к своему сигнальщику
    virtual MarketDataServiceSignals* signaller() const = 0;

    // Запрос инструментов
    virtual void requestInstruments(const Tools::InstrumentsInfoRequest& req) = 0;
    // Запрос свеч
    virtual void requestKlines(const Tools::KlinesRequest& req) = 0;

};

}