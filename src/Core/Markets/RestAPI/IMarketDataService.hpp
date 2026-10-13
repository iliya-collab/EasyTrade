#pragma once
#include "MarketDataServiceSignals.hpp"
#include "Tools/QueryStructures.hpp"

namespace Core::Markets
{
    class IMarketDataService : public QObject
    {
        Q_OBJECT
    public:

        IMarketDataService(QObject* parent = nullptr) : QObject(parent) {}

        virtual void init(bool isTestnet) = 0;

        // Доступ к своему сигнальщику
        virtual MarketDataServiceSignals* signaller() = 0;

        // Запрос пар
        virtual void requestTradePairs(Tools::EnumHelpers::MarketType category) = 0;
        // Запрос свеч
        virtual void requestKlines(const Tools::KlinesRequest& req) = 0;

    };
}