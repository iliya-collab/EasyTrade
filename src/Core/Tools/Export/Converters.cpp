#include "Converters.hpp"

QJsonObject Core::Tools::Exporter::Converters::toJson(const PublicTradeItem &t)
{
    QJsonObject o;
    o["category"] = marketTypeToString(t.m_category);
    o["symbol"]   = t.m_symbol;
    o["side"]     = orderSideToString(t.m_side);
    o["price"]    = t.m_price;
    o["volume"]   = t.m_volume;
    o["turnover"] = t.m_turnover;
    o["time"]     = static_cast<qint64>(t.m_tradeTime);
    return o;
}

QList<QPair<QString, QString> > Core::Tools::Exporter::Converters::toXmlFields(const PublicTradeItem &t)
{
    return {
        { "category", marketTypeToString(t.m_category) },
        { "symbol",   t.m_symbol },
        { "side",     orderSideToString(t.m_side) },
        { "price",    QString::number(t.m_price, 'f', 8) },
        { "volume",   QString::number(t.m_volume, 'f', 8) },
        { "turnover", QString::number(t.m_turnover, 'f', 8) },
        { "time",     QString::number(t.m_tradeTime) },
    };
}

QJsonObject Core::Tools::Exporter::Converters::toJson(const Kline &k)
{

}

QList<QPair<QString, QString> > Core::Tools::Exporter::Converters::toXmlFields(const Kline &k)
{

}
