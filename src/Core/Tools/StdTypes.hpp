#pragma once
#include "StdEnums.hpp"
#include <QString>
#include <QList>
#include <QMap>
#include <QObject>
#include <QVariant>
#include <QtQml>

namespace Core::Tools
{

// ==========================================================================================
//  Баланс аккаунта
// ==========================================================================================

struct AccountBalance
{
public:

    double m_totalWalletBalance = 0; // Общий баланс кошелька аккаунта в USD
    std::vector<std::tuple<QString, double, double>> m_assets{}; // активы (монета, кол-во, стоимость)

};

// ==========================================================================================
//  Стакан заявок
// ==========================================================================================

using OrderbookSide = QMap<double, double>; // уровни (цена, объем)

struct Orderbook {

    MarketType m_category = MarketType::Unknown;
    QString m_symbol = ""; // Название пары
    OrderbookSide m_bids{}; // Покупки
    OrderbookSide m_asks{}; // Продажи

};

// ==========================================================================================
//  Тикер
// ==========================================================================================

struct Ticker {
    Q_GADGET
    QML_VALUE_TYPE(Ticker)

    Q_PROPERTY(Core::Tools::MarketType category MEMBER m_category FINAL)
    Q_PROPERTY(QString symbol MEMBER m_symbol FINAL)
    Q_PROPERTY(QString lastPrice MEMBER m_lastPrice FINAL)
    Q_PROPERTY(QString usdIndexPrice MEMBER m_usdIndexPrice FINAL)
    Q_PROPERTY(QString high24h MEMBER m_high24h FINAL)
    Q_PROPERTY(QString low24h MEMBER m_low24h FINAL)
    Q_PROPERTY(QString volCcy24h MEMBER m_volCcy24h FINAL)
    Q_PROPERTY(QString vol24h MEMBER m_vol24h FINAL)
    Q_PROPERTY(QString prevPrice24h MEMBER m_prevPrice24h FINAL)
    Q_PROPERTY(QString price24hPcnt MEMBER m_price24hPcnt FINAL)

public:

    MarketType m_category = MarketType::Unknown;
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

// ==========================================================================================
//  Свеча
// ==========================================================================================

struct Kline {

    MarketType m_category = MarketType::Unknown;
    QString m_symbol = "";
    Interval m_interval = Interval::Unknown;
    QString m_open {};
    QString m_close {};
    QString m_high {};
    QString m_low {};
    qint64 m_start {};
    qint64 m_end {};
    QString m_volume {};
    QString m_turnover {};
    bool m_confirm = false;

};

// ==========================================================================================
//  Публичная лента сделок
// ==========================================================================================

struct PublicTradeItem {

    MarketType m_category = MarketType::Unknown;
    QString m_symbol = "";
    OrderSide m_side = OrderSide::Unknown;
    QString m_price {};
    QString m_volume {};
    QString m_turnover {};
    qint64 m_tradeTime = 0;

};

using PublicTrades = QList<PublicTradeItem>;

// ==========================================================================================
//  API-ключ и информация о нём
// ==========================================================================================

struct ApiInfo {
    Q_GADGET
    QML_VALUE_TYPE(ApiInfo)

    Q_PROPERTY(bool readOnly MEMBER m_readOnly FINAL)
    Q_PROPERTY(bool permissionSpotTrade MEMBER m_permissionSpotTrade FINAL)
    Q_PROPERTY(bool permissionWithdraw MEMBER m_permissionWithdraw FINAL)
    Q_PROPERTY(bool permissionContractTrade MEMBER m_permissionContractTrade FINAL)
    Q_PROPERTY(bool permissionAccountTransfer MEMBER m_permissionAccountTransfer FINAL)
    Q_PROPERTY(QString expiredAt MEMBER m_expiredAt FINAL)
    Q_PROPERTY(QStringList ips MEMBER m_ips FINAL)

public:

    QList<QString> m_ips{};                     // IP адреса, которые могут исп этот ключ
    QString m_expiredAt = "";                   // Срок истечения
    bool m_readOnly = true;                     // Только для чтения
    bool m_permissionSpotTrade = false;         // Разрешение к спотовой торговли
    bool m_permissionContractTrade = false;     // Разрешение к фьючерсаной торговли
    bool m_permissionWithdraw = false;          // Разрешение к выводу активов с биржи
    bool m_permissionAccountTransfer = false;   // Разрешение к переводу между внутренними кошельками

};

struct Api {
    Q_GADGET
    QML_VALUE_TYPE(Api)

    Q_PROPERTY(QString apiKey MEMBER m_apiKey FINAL)
    Q_PROPERTY(QString secretKey MEMBER m_secretKey FINAL)
    Q_PROPERTY(bool isTestnet MEMBER m_isTestnet FINAL)

public:

    QString m_apiKey = "";              // Api ключ
    QString m_secretKey = "";           // Секретный Api ключ
    bool m_isTestnet = false;           // Тип сети

};

// ==========================================================================================
//  Торговая пара как инструмент
// ==========================================================================================

struct TradeInfo {

    MarketType m_category = MarketType::Unknown;
    QString m_symbol = "";
    QString m_baseCoin = "";
    QString m_quoteCoin = "";

};

// ==========================================================================================
//  Ордер / исполнение / позиция (приватный WS-стрим)
// ==========================================================================================

// Информация об ордере
struct OrderInfo
{
    Q_GADGET
    QML_VALUE_TYPE(OrderInfo)

    Q_PROPERTY(Core::Tools::MarketType category MEMBER m_category FINAL)
    Q_PROPERTY(QString orderId MEMBER m_orderId FINAL)
    Q_PROPERTY(QString orderLinkId MEMBER m_orderLinkId FINAL)
    Q_PROPERTY(QString symbol MEMBER m_symbol FINAL)
    Q_PROPERTY(Core::Tools::OrderSide side MEMBER m_side FINAL)
    Q_PROPERTY(Core::Tools::OrderType orderType MEMBER m_orderType FINAL)
    Q_PROPERTY(QString price MEMBER m_price FINAL)
    Q_PROPERTY(QString qty MEMBER m_qty FINAL)
    Q_PROPERTY(Core::Tools::OrderStatus status MEMBER m_status FINAL)
    Q_PROPERTY(int positionIdx MEMBER m_positionIdx FINAL)
    Q_PROPERTY(QString leavesQty MEMBER m_leavesQty FINAL)
    Q_PROPERTY(QString cumExecQty MEMBER m_cumExecQty FINAL)
    Q_PROPERTY(QString cumExecValue MEMBER m_cumExecValue FINAL)
    Q_PROPERTY(QString cumExecFee MEMBER m_cumExecFee FINAL)
    Q_PROPERTY(QString rejectReason MEMBER m_rejectReason FINAL)
    Q_PROPERTY(qint64 createdTime MEMBER m_createdTime FINAL)
    Q_PROPERTY(qint64 updatedTime MEMBER m_updatedTime FINAL)

public:

    MarketType m_category = MarketType::Unknown; // Категория рынка
    QString m_orderId; // Уникальный идентификатор ордера, присвоенный биржей Bybit
    QString m_orderLinkId; // Клиентский идентификатор ордера (задается вами при отправке для локального трекинга).
    QString m_symbol; // Торговая пара (например, "BTCUSDT").
    OrderSide m_side = OrderSide::Unknown; // Направление ордера (Buy или Sell).
    OrderType m_orderType; // Тип ордера (Limit, Market).
    QString m_price; // Цена, указанная при выставлении ордера (для рыночных ордеров обычно 0.0 или цена скольжения).
    QString m_qty; // Изначальный (заявленный) объем ордера в базовой валюте.
    OrderStatus m_status = OrderStatus::Unknown; // Текущий статус ордера (New, Filled и т.д.).
    int m_positionIdx = 0; // Режим позиции (0-OneWay, 1-Hedge Long, 2-Hedge Short); актуально для деривативов в хедж-режиме.
    QString m_leavesQty; // Сколько еще осталось исполнить.
    QString m_cumExecQty; // Сколько суммарно уже исполнено.
    QString m_cumExecValue; // Суммарная стоимость исполненной части.
    QString m_cumExecFee; // Накопленная комиссия по ордеру.
    QString m_rejectReason; // Причина отклонения, если статус Rejected.
    qint64 m_createdTime = 0; // Таймстемп создания (мс).
    qint64 m_updatedTime = 0; // Таймстемп последнего изменения (мс).
};

struct ExecutionInfo
{
    Q_GADGET
    QML_VALUE_TYPE(ExecutionInfo)

    Q_PROPERTY(Core::Tools::MarketType category MEMBER m_category FINAL)
    Q_PROPERTY(QString symbol MEMBER m_symbol FINAL)
    Q_PROPERTY(QString execId MEMBER m_execId FINAL)
    Q_PROPERTY(QString orderId MEMBER m_orderId FINAL)
    Q_PROPERTY(QString orderLinkId MEMBER m_orderLinkId FINAL)
    Q_PROPERTY(Core::Tools::OrderSide side MEMBER m_side FINAL)
    Q_PROPERTY(QString execPrice MEMBER m_execPrice FINAL)
    Q_PROPERTY(QString execQty MEMBER m_execQty FINAL)
    Q_PROPERTY(QString execValue MEMBER m_execValue FINAL)
    Q_PROPERTY(Core::Tools::ExecType execType MEMBER m_execType FINAL)
    Q_PROPERTY(QString execFee MEMBER m_execFee FINAL)
    Q_PROPERTY(QString feeCurrency MEMBER m_feeCurrency FINAL)
    Q_PROPERTY(bool isMaker MEMBER m_isMaker FINAL)
    Q_PROPERTY(qint64 execTime MEMBER m_execTime FINAL)

public:

    MarketType m_category = MarketType::Unknown; // Категория рынка
    QString m_symbol; // Торговая пара
    QString m_execId; // Уникальный ID транзакции (филла)
    QString m_orderId; // ID родительского ордера
    QString m_orderLinkId; // Ваш клиентский ID
    OrderSide m_side = OrderSide::Unknown; // Buy, Sell

    QString m_execPrice; // Цена этой конкретной сделки
    QString m_execQty; // Объем этой конкретной сделки
    QString m_execValue; // Стоимость этой конкретной сделки

    ExecType m_execType = ExecType::Unknown; // Trade, Adl, Funding, Liquidation
    QString m_execFee; // Комиссия за эту сделку (отрицательная = рибейт)
    QString m_feeCurrency; // Валюта комиссии (USDT, BTC и т.д.)
    bool m_isMaker = false; // true = Maker, false = Taker
    qint64 m_execTime = 0; // Время совершения сделки (мс)
};

struct PositionInfo
{
    Q_GADGET
    QML_VALUE_TYPE(PositionInfo)

    Q_PROPERTY(Core::Tools::MarketType category MEMBER m_category FINAL)
    Q_PROPERTY(QString symbol MEMBER m_symbol FINAL)
    Q_PROPERTY(Core::Tools::OrderSide side MEMBER m_side FINAL)
    Q_PROPERTY(QString size MEMBER m_size FINAL)
    Q_PROPERTY(int positionIdx MEMBER m_positionIdx FINAL)
    Q_PROPERTY(QString avgPrice MEMBER m_avgPrice FINAL)
    Q_PROPERTY(QString liqPrice MEMBER m_liqPrice FINAL)
    Q_PROPERTY(QString markPrice MEMBER m_markPrice FINAL)
    Q_PROPERTY(QString leverage MEMBER m_leverage FINAL)
    Q_PROPERTY(QString positionIM MEMBER m_positionIM FINAL)
    Q_PROPERTY(QString positionMM MEMBER m_positionMM FINAL)
    Q_PROPERTY(QString unrealisedPnl MEMBER m_unrealisedPnl FINAL)
    Q_PROPERTY(QString cumRealisedPnl MEMBER m_cumRealisedPnl FINAL)
    Q_PROPERTY(int adlRankIndicator MEMBER m_adlRankIndicator FINAL)

public:

    MarketType m_category = MarketType::Unknown; // Категория рынка (linear, inverse)
    QString m_symbol; // Торговая пара
    OrderSide m_side = OrderSide::Unknown; // Buy=Long, Sell=Short, Unknown=нет позиции
    QString m_size; // Текущий размер открытой позиции (0 если закрыта)
    int m_positionIdx = 0; // Режим: 0-OneWay, 1-Hedge Long, 2-Hedge Short

    // Финансовые метрики риска
    QString m_avgPrice; // Средняя цена входа
    QString m_liqPrice; // Цена ликвидации
    QString m_markPrice; // Текущая цена маркировки
    QString m_leverage; // Плечо
    QString m_positionIM; // Начальная маржа под позицию
    QString m_positionMM; // Поддерживающая маржа

    // Результат
    QString m_unrealisedPnl; // Нереализованный PnL
    QString m_cumRealisedPnl; // Накопленный реализованный PnL

    int m_adlRankIndicator = 0; // Очередь на авто-делеверидж (0-5)
};

}

// РЕГИСТРАЦИЯ В МЕТАСИСТЕМЕ QT
Q_DECLARE_METATYPE(Core::Tools::AccountBalance)
Q_DECLARE_METATYPE(Core::Tools::Orderbook)
Q_DECLARE_METATYPE(Core::Tools::Ticker)
Q_DECLARE_METATYPE(Core::Tools::Kline)
Q_DECLARE_METATYPE(Core::Tools::PublicTradeItem)
Q_DECLARE_METATYPE(Core::Tools::PublicTrades)
Q_DECLARE_METATYPE(Core::Tools::Api)
Q_DECLARE_METATYPE(Core::Tools::ApiInfo)
Q_DECLARE_METATYPE(Core::Tools::TradeInfo)
Q_DECLARE_METATYPE(Core::Tools::OrderInfo)
Q_DECLARE_METATYPE(Core::Tools::ExecutionInfo)
Q_DECLARE_METATYPE(Core::Tools::PositionInfo)