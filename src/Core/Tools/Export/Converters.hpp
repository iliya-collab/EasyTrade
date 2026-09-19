#pragma once
#include "Tools/StdTypes.hpp"
#include <QJsonObject>
#include <QPair>

namespace Core::Tools::Exporter::Converters
{

// --- PublicTradeItem ---
inline QList<QPair<QString, QVariant>> toField(const PublicTradeItem& t);

// --- Kline ---
inline QList<QPair<QString, QVariant>> toField(const Kline& k);

// --- OrderInfo ---
inline QList<QPair<QString, QVariant>> toField(const OrderInfo& o);

// --- ExecutionInfo ---
inline QList<QPair<QString, QVariant>> toField(const ExecutionInfo& e);

// --- PositionInfo ---
inline QList<QPair<QString, QVariant>> toField(const PositionInfo& p);

}
