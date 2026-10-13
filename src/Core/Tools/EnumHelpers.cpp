#include "EnumHelpers.hpp"

namespace detail
{

    template <typename E>
    QString enumToString(E value)
    {
        const QMetaEnum me = QMetaEnum::fromType<E>();
        Q_ASSERT_X(me.isValid(), "enumToString", "Enum is not registered in Qt meta-system");
        if (!me.isValid())
            return {};
        const char* key = me.valueToKey(static_cast<int>(value));
        return key ? QString::fromLatin1(key) : QString();
    }

    template <typename E>
    E stringToEnum(const QString& str)
    {
        const QMetaEnum me = QMetaEnum::fromType<E>();
        if (!me.isValid())
            return static_cast<E>(0);

        bool ok = false;
        const int v = me.keyToValue(str.toLatin1().constData(), &ok);
        if (ok)
            return static_cast<E>(v);

        const int unknown = me.keyToValue("Unknown");
        return unknown >= 0 ? static_cast<E>(unknown) : static_cast<E>(0);
    }

}

qint64 Core::Tools::EnumHelpers::intervalToMs(Interval interval)
{
    switch (interval)
    {
    case Interval::Min1:     return 60'000LL;
    case Interval::Min3:     return 3 * 60'000LL;
    case Interval::Min5:     return 5 * 60'000LL;
    case Interval::Min15:    return 15 * 60'000LL;
    case Interval::Min30:    return 30 * 60'000LL;
    case Interval::Hour1:    return 60 * 60'000LL;
    case Interval::Hour2:    return 2 * 60 * 60'000LL;
    case Interval::Hour4:    return 4 * 60 * 60'000LL;
    case Interval::Hour6:    return 6 * 60 * 60'000LL;
    case Interval::Hour12:   return 12 * 60 * 60'000LL;
    case Interval::Day:      return 24 * 60 * 60'000LL;
    case Interval::Week:     return 7 * 24 * 60 * 60'000LL;
    case Interval::Month:    return 30LL * 24 * 60 * 60'000LL;
    case Interval::Unknown:
    default:
        Q_ASSERT_X(false, "intervalToMs", "Unknown interval");
        return 0;
    }
}

QString Core::Tools::EnumHelpers::intervalToString(Interval interval)
{
    switch (interval)
    {
    case Interval::Min1:    return "1";
    case Interval::Min3:    return "3";
    case Interval::Min5:    return "5";
    case Interval::Min15:   return "15";
    case Interval::Min30:   return "30";
    case Interval::Hour1:   return "60";
    case Interval::Hour2:   return "120";
    case Interval::Hour4:   return "240";
    case Interval::Hour6:   return "360";
    case Interval::Hour12:  return "720";
    case Interval::Day:     return "D";
    case Interval::Week:    return "W";
    case Interval::Month:   return "M";
    case Interval::Unknown:
    default:
        Q_ASSERT_X(false, "intervalToString", "Unknown interval");
        return "";
    }
}

Core::Tools::EnumHelpers::Interval Core::Tools::EnumHelpers::stringToInterval(const QString &intervalStr)
{
    if (intervalStr == "1")   return Interval::Min1;
    if (intervalStr == "3")   return Interval::Min3;
    if (intervalStr == "5")   return Interval::Min5;
    if (intervalStr == "15")  return Interval::Min15;
    if (intervalStr == "30")  return Interval::Min30;

    if (intervalStr == "60")  return Interval::Hour1;
    if (intervalStr == "120") return Interval::Hour2;
    if (intervalStr == "240") return Interval::Hour4;
    if (intervalStr == "360") return Interval::Hour6;
    if (intervalStr == "720") return Interval::Hour12;

    if (intervalStr == "D")   return Interval::Day;
    if (intervalStr == "W")   return Interval::Week;
    if (intervalStr == "M")   return Interval::Month;

    return Interval::Unknown;
}

QString Core::Tools::EnumHelpers::marketTypeToApiString(MarketType type)
{
    switch (type)
    {
    case MarketType::Spot:      return "spot";
    case MarketType::Linear:    return "linear";
    case MarketType::Inverse:   return "inverse";
    case MarketType::Option:    return "option";
    case MarketType::Unknown:
    default:
        Q_ASSERT_X(false, "marketTypeToApiString", "Unknown market type");
        return "";
    }
}

Core::Tools::EnumHelpers::MarketType Core::Tools::EnumHelpers::apiStringToMarketType(const QString& type)
{
    if (type == "spot")     return MarketType::Spot;
    if (type == "linear")   return MarketType::Linear;
    if (type == "inverse")  return MarketType::Inverse;
    if (type == "option")   return MarketType::Option;

    return MarketType::Unknown;
}

QString Core::Tools::EnumHelpers::orderSideToString(OrderSide side)
{
    return detail::enumToString(side);
}

Core::Tools::EnumHelpers::OrderSide Core::Tools::EnumHelpers::stringToOrderSide(const QString& side)
{
    return detail::stringToEnum<OrderSide>(side);
}

QString Core::Tools::EnumHelpers::orderStatusToString(OrderStatus status)
{
    return detail::enumToString(status);
}

Core::Tools::EnumHelpers::OrderStatus Core::Tools::EnumHelpers::stringToOrderStatus(const QString& status)
{
    return detail::stringToEnum<OrderStatus>(status);
}

QString Core::Tools::EnumHelpers::execTypeToString(ExecType type)
{
    return detail::enumToString(type);
}

Core::Tools::EnumHelpers::ExecType Core::Tools::EnumHelpers::stringToExecType(const QString& type)
{
    return detail::stringToEnum<ExecType>(type);
}

QString Core::Tools::EnumHelpers::orderTypeToString(OrderType type)
{
    return detail::enumToString(type);
}

Core::Tools::EnumHelpers::OrderType Core::Tools::EnumHelpers::stringToOrderType(const QString& type)
{
    return detail::stringToEnum<OrderType>(type);
}

QString Core::Tools::EnumHelpers::timeInForceToString(TimeInForce tif)
{
    return detail::enumToString(tif);
}

Core::Tools::EnumHelpers::TimeInForce Core::Tools::EnumHelpers::stringToTimeInForce(const QString& tif)
{
    return detail::stringToEnum<TimeInForce>(tif);
}
