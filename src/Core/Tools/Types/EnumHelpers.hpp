#pragma once
#include "RegistrationToolsTypes.hpp"
#include <QObject>

namespace Core::Tools::Types
{

class EnumHelpers : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")

public:

    enum class Interval
    {
        Min1,
        Min3,
        Min5,
        Min15,
        Min30,
        Hour1,
        Hour2,
        Hour4,
        Hour6,
        Hour12,
        Day,
        Week,
        Month,
        Unknown
    };
    Q_ENUM(Interval)

    // ==========================================================================================
    //  Тип рынка / категория инструмента
    // ==========================================================================================

    enum class MarketType
    {
        Spot,
        Linear,
        Unknown
    };
    Q_ENUM(MarketType)

    // ==========================================================================================
    //  Направление / статус ордера / тип исполнения
    // ==========================================================================================

    // Направление операции
    enum class OrderSide
    {
        Buy, // Покупка. Для спота — приобретение базового актива. Для фьючерсов — открытие Long-позиции или закрытие Short-позиции.
        Sell, // Продажа. Для спота — продажа актива. Для фьючерсов — открытие Short-позиции или закрытие Long-позиции.
        Unknown // Неизвестное направление. Защитное значение на случай десериализации некорректных данных или при полном закрытии позиции (когда Bybit присылает side: "").
    };
    Q_ENUM(OrderSide)

    // Тип ордера при выставлении
    enum class OrderType
    {
        Limit, // Лимитный ордер — исполняется по указанной цене (m_price обязателен).
        Market, // Рыночный ордер — исполняется по текущей рыночной цене немедленно.
        Unknown
    };
    Q_ENUM(OrderType)

    enum class OrderStatus
    {
        New, // Ордер успешно принят торговым движком биржи и размещен в стакане (активен).
        PartiallyFilled, // Ордер частично исполнен. Часть объема уже превратилась в сделки, но остаток (m_leavesQty) всё еще находится в стакане.
        Untriggered,
        Rejected, // Ордер отклонен биржей на этапе валидации (например, не хватило маржи, неверная цена, сработал риск-лимит) и не попал в стакан.
        PartiallyFilledCanceled,
        Filled, // Ордер полностью исполнен. Весь заявленный объем (m_qty) был сведен со встречными ордерами. Ордер неактивен.
        Cancelled, // Ордер отменен пользователем или системой (например, по истечении времени действия или по правилу Post-Only), до того как был полностью исполнен.
        Triggered,
        Deactivated,
        Unknown // Неизвестный статус (или редкие специфичные статусы Bybit вроде Deactivated, если они не смаппированы отдельно).
    };
    Q_ENUM(OrderStatus)

    enum class ExecType
    {
        Trade, // Обычная торговая сделка на бирже (ордер сопоставился со встречным ордером в стакане).
        Adl, // Авто-делеверидж (Auto-Deleveraging). Принудительное закрытие вашей прибыльной позиции встречным ордером ликвидируемого игрока при нехватке страхового фонда биржи.
        Funding, // Периодическое списание или начисление комиссии за удержание позиций на бессрочных фьючерсах (обычно каждые 4-8 часов).
        Liquidation, // Сделка, совершенная в процессе принудительной ликвидации позиции из-за нехватки поддерживающей маржи.
        Unknown // Неизвестный тип исполнения.
    };
    Q_ENUM(ExecType)

    // Время действия ордера
    enum class TimeInForce
    {
        GTC, // Good Till Cancel — действует, пока не будет исполнен или отменен вручную (по умолчанию).
        IOC, // Immediate Or Cancel — исполняется немедленно (полностью или частично), неисполненный остаток отменяется.
        FOK, // Fill Or Kill — исполняется полностью и немедленно, либо отменяется целиком.
        PostOnly, // Только мейкер — ордер отклоняется, если исполнился бы сразу как тейкер.
        Unknown
    };
    Q_ENUM(TimeInForce)

    explicit EnumHelpers(QObject* parent = nullptr) : QObject(parent) {};

    Q_INVOKABLE static qint64 intervalToMs(Interval interval);
    Q_INVOKABLE static QString intervalToString(Interval interval);
    Q_INVOKABLE static Interval stringToInterval(const QString &intervalStr);

    Q_INVOKABLE static QString marketTypeToApiString(MarketType type);
    Q_INVOKABLE static MarketType apiStringToMarketType(const QString& type);

    Q_INVOKABLE static QString orderSideToString(OrderSide side);
    Q_INVOKABLE static OrderSide stringToOrderSide(const QString& side);

    Q_INVOKABLE static QString orderStatusToString(OrderStatus status);
    Q_INVOKABLE static OrderStatus stringToOrderStatus(const QString& status);

    Q_INVOKABLE static QString execTypeToString(ExecType type);
    Q_INVOKABLE static ExecType stringToExecType(const QString& type);

    Q_INVOKABLE static QString orderTypeToString(OrderType type);
    Q_INVOKABLE static OrderType stringToOrderType(const QString& type);

    Q_INVOKABLE static QString timeInForceToString(TimeInForce tif);
    Q_INVOKABLE static TimeInForce stringToTimeInForce(const QString& tif);

};

}
Q_DECLARE_METATYPE(Core::Tools::Types::EnumHelpers::Interval)
Q_DECLARE_METATYPE(Core::Tools::Types::EnumHelpers::MarketType)
Q_DECLARE_METATYPE(Core::Tools::Types::EnumHelpers::OrderType)
Q_DECLARE_METATYPE(Core::Tools::Types::EnumHelpers::OrderSide)
Q_DECLARE_METATYPE(Core::Tools::Types::EnumHelpers::OrderStatus)
Q_DECLARE_METATYPE(Core::Tools::Types::EnumHelpers::ExecType)


