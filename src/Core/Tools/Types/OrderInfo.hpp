#ifndef ORDERINFO_HPP
#define ORDERINFO_HPP
#include "EnumHelpers.hpp"

namespace Core::Tools::Types
{

// ==========================================================================================
//  Информация об ордере
// ==========================================================================================

class OrderInfo
{
    Q_GADGET
    QML_VALUE_TYPE(tOrderInfo)

    Q_PROPERTY(EnumHelpers::MarketType category MEMBER m_category FINAL)
    Q_PROPERTY(QString orderId MEMBER m_orderId FINAL)
    Q_PROPERTY(QString orderLinkId MEMBER m_orderLinkId FINAL)
    Q_PROPERTY(QString symbol MEMBER m_symbol FINAL)
    Q_PROPERTY(EnumHelpers::OrderSide side MEMBER m_side FINAL)
    Q_PROPERTY(EnumHelpers::OrderType orderType MEMBER m_orderType FINAL)
    Q_PROPERTY(QString price MEMBER m_price FINAL)
    Q_PROPERTY(QString qty MEMBER m_qty FINAL)
    Q_PROPERTY(EnumHelpers::OrderStatus status MEMBER m_status FINAL)
    Q_PROPERTY(int positionIdx MEMBER m_positionIdx FINAL)
    Q_PROPERTY(QString leavesQty MEMBER m_leavesQty FINAL)
    Q_PROPERTY(QString cumExecQty MEMBER m_cumExecQty FINAL)
    Q_PROPERTY(QString cumExecValue MEMBER m_cumExecValue FINAL)
    Q_PROPERTY(QString cumExecFee MEMBER m_cumExecFee FINAL)
    Q_PROPERTY(QString rejectReason MEMBER m_rejectReason FINAL)
    Q_PROPERTY(qint64 createdTime MEMBER m_createdTime FINAL)
    Q_PROPERTY(qint64 updatedTime MEMBER m_updatedTime FINAL)

public:

    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown; // Категория рынка
    QString m_orderId; // Уникальный идентификатор ордера, присвоенный биржей Bybit
    QString m_orderLinkId; // Клиентский идентификатор ордера (задается вами при отправке для локального трекинга).
    QString m_symbol; // Торговая пара (например, "BTCUSDT").
    EnumHelpers::OrderSide m_side = EnumHelpers::OrderSide::Unknown; // Направление ордера (Buy или Sell).
    EnumHelpers::OrderType m_orderType; // Тип ордера (Limit, Market).
    QString m_price; // Цена, указанная при выставлении ордера (для рыночных ордеров обычно 0.0 или цена скольжения).
    QString m_qty; // Изначальный (заявленный) объем ордера в базовой валюте.
    EnumHelpers::OrderStatus m_status = EnumHelpers::OrderStatus::Unknown; // Текущий статус ордера (New, Filled и т.д.).
    int m_positionIdx = 0; // Режим позиции (0-OneWay, 1-Hedge Long, 2-Hedge Short); актуально для деривативов в хедж-режиме.
    QString m_leavesQty; // Сколько еще осталось исполнить.
    QString m_cumExecQty; // Сколько суммарно уже исполнено.
    QString m_cumExecValue; // Суммарная стоимость исполненной части.
    QString m_cumExecFee; // Накопленная комиссия по ордеру.
    QString m_rejectReason; // Причина отклонения, если статус Rejected.
    qint64 m_createdTime = 0; // Таймстемп создания (мс).
    qint64 m_updatedTime = 0; // Таймстемп последнего изменения (мс).

    bool isSameOrder(const OrderInfo& other) const
    {
        return (!m_orderId.isEmpty() && m_orderId == other.m_orderId) ||
               (!m_orderLinkId.isEmpty() && m_orderLinkId == other.m_orderLinkId);
    }

    OrderInfo& operator=(const OrderInfo& other) {
        if (this == &other)
            return *this;

        this->m_category = other.m_category;
        this->m_orderId = other.m_orderId;
        this->m_orderLinkId = other.m_orderLinkId;
        this->m_symbol = other.m_symbol;
        this->m_side= other.m_side;
        this->m_orderType= other.m_orderType;
        this->m_price= other.m_price;
        this->m_qty= other.m_qty;
        this->m_status= other.m_status;
        this->m_positionIdx= other.m_positionIdx;
        this->m_leavesQty= other.m_leavesQty;
        this->m_cumExecQty= other.m_cumExecQty;
        this->m_cumExecValue= other.m_cumExecValue;
        this->m_cumExecFee= other.m_cumExecFee;
        this->m_rejectReason= other.m_rejectReason;
        this->m_createdTime= other.m_createdTime;
        this->m_updatedTime= other.m_updatedTime;

        return *this;
    }

    bool operator==(const OrderInfo& other) const
    {
        return std::tie(m_category, m_orderId, m_orderLinkId, m_symbol, m_side,
                        m_orderType, m_price, m_qty, m_status, m_positionIdx,
                        m_leavesQty, m_cumExecQty, m_cumExecValue, m_cumExecFee,
                        m_rejectReason, m_createdTime, m_updatedTime)
               == std::tie(other.m_category, other.m_orderId, other.m_orderLinkId, other.m_symbol, other.m_side,
                           other.m_orderType, other.m_price, other.m_qty, other.m_status, other.m_positionIdx,
                           other.m_leavesQty, other.m_cumExecQty, other.m_cumExecValue, other.m_cumExecFee,
                           other.m_rejectReason, other.m_createdTime, other.m_updatedTime);
    }

    bool operator!=(const OrderInfo& other) const
    {
        return !(*this == other);
    }

};

using Orders = QList<OrderInfo>;

}

#endif // ORDERINFO_HPP
