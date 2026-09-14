#pragma once
#include "Tools/StdTypes.hpp"
#include <QJsonObject>
#include <QPair>

namespace Core::Tools::Exporter::Converters
{

// --- PublicTradeItem ---
inline QJsonObject toJson(const PublicTradeItem& t);
inline QList<QPair<QString, QString>> toXmlFields(const PublicTradeItem& t);

// --- Kline ---
inline QJsonObject toJson(const Kline& k);
inline QList<QPair<QString, QString>> toXmlFields(const Kline& k);

}
