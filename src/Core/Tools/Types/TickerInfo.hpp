#ifndef TICKER_HPP
#define TICKER_HPP
#include "EnumHelpers.hpp"

namespace Core::Tools::Types
{

// ==========================================================================================
//  Тикер
// ==========================================================================================

struct TickerInfo
{
    Q_GADGET
    QML_VALUE_TYPE(tTickerInfo)

    Q_PROPERTY(EnumHelpers::MarketType                  category        MEMBER m_category FINAL)
    Q_PROPERTY(QString                                  symbol          MEMBER m_symbol FINAL)
    Q_PROPERTY(QString                                  lastPrice       MEMBER m_lastPrice FINAL)
    Q_PROPERTY(QString                                  usdIndexPrice   MEMBER m_usdIndexPrice FINAL)
    Q_PROPERTY(QString                                  high24h         MEMBER m_high24h FINAL)
    Q_PROPERTY(QString                                  low24h          MEMBER m_low24h FINAL)
    Q_PROPERTY(QString                                  volCcy24h       MEMBER m_volCcy24h FINAL)
    Q_PROPERTY(QString                                  vol24h          MEMBER m_vol24h FINAL)
    Q_PROPERTY(QString                                  prevPrice24h    MEMBER m_prevPrice24h FINAL)
    Q_PROPERTY(QString                                  price24hPcnt    MEMBER m_price24hPcnt FINAL)

public:

    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown;
    QString m_symbol = ""; // Название пары
    QString m_lastPrice {}; // Последняя цена сделки
    QString m_usdIndexPrice {}; // Индексная цена доллара США
    QString m_high24h {}; // Максимальная цена за 24 часа
    QString m_low24h {}; // Минимальная цена за 24 часа
    QString m_volCcy24h {}; // Объем торгов в котируемой валюте за 24ч (USDT)
    QString m_vol24h {}; // Объем торгов в базовой валюте за 24ч (BTC)
    QString m_prevPrice24h {}; // Рыночная цена 24 часа назад
    QString m_price24hPcnt {}; // Процентное изменение рыночной цены относительно 24 часов

};

}

#endif // TICKER_HPP
