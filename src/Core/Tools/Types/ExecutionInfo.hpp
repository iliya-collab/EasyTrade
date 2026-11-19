#ifndef EXECUTIONINFO_HPP
#define EXECUTIONINFO_HPP
#include "EnumHelpers.hpp"

namespace Core::Tools::Types
{

// ==========================================================================================
//  Информация об исполнении
// ==========================================================================================

struct ExecutionInfo
{
    Q_GADGET
    QML_VALUE_TYPE(tExecutionInfo)

    Q_PROPERTY(EnumHelpers::MarketType category MEMBER m_category FINAL)
    Q_PROPERTY(QString symbol MEMBER m_symbol FINAL)
    Q_PROPERTY(QString execId MEMBER m_execId FINAL)
    Q_PROPERTY(QString orderId MEMBER m_orderId FINAL)
    Q_PROPERTY(QString orderLinkId MEMBER m_orderLinkId FINAL)
    Q_PROPERTY(EnumHelpers::OrderSide side MEMBER m_side FINAL)
    Q_PROPERTY(QString execPrice MEMBER m_execPrice FINAL)
    Q_PROPERTY(QString execQty MEMBER m_execQty FINAL)
    Q_PROPERTY(QString execValue MEMBER m_execValue FINAL)
    Q_PROPERTY(EnumHelpers::ExecType execType MEMBER m_execType FINAL)
    Q_PROPERTY(QString execFee MEMBER m_execFee FINAL)
    Q_PROPERTY(QString feeCurrency MEMBER m_feeCurrency FINAL)
    Q_PROPERTY(bool isMaker MEMBER m_isMaker FINAL)
    Q_PROPERTY(qint64 execTime MEMBER m_execTime FINAL)

public:

    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown; // Категория рынка
    QString m_symbol; // Торговая пара
    QString m_execId; // Уникальный ID транзакции (филла)
    QString m_orderId; // ID родительского ордера
    QString m_orderLinkId; // Ваш клиентский ID
    EnumHelpers::OrderSide m_side = EnumHelpers::OrderSide::Unknown; // Buy, Sell

    QString m_execPrice; // Цена этой конкретной сделки
    QString m_execQty; // Объем этой конкретной сделки
    QString m_execValue; // Стоимость этой конкретной сделки

    EnumHelpers::ExecType m_execType = EnumHelpers::ExecType::Unknown; // Trade, Adl, Funding, Liquidation
    QString m_execFee; // Комиссия за эту сделку (отрицательная = рибейт)
    QString m_feeCurrency; // Валюта комиссии (USDT, BTC и т.д.)
    bool m_isMaker = false; // true = Maker, false = Taker
    qint64 m_execTime = 0; // Время совершения сделки (мс)
};

using Executions = QList<ExecutionInfo>;

}

#endif // EXECUTIONINFO_HPP
