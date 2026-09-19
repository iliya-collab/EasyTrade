#pragma once
#include <qobject.h>

namespace Core::Tools::Exporter
{
Q_NAMESPACE

enum class ExportFormat
{
    Json,
    Xml,
    Csv,
    Yaml,
    Unknown
};
Q_ENUM_NS(ExportFormat)

inline QString exportFormatToString(ExportFormat fmt)
{
    switch (fmt)
    {
    case ExportFormat::Json:    return "json";
    case ExportFormat::Xml:     return "xml";
    case ExportFormat::Csv:     return "csv";
    case ExportFormat::Yaml:    return "yaml";
    default:                    return "";
    }
}

inline ExportFormat stringToExportFormat(const QString &str)
{
    if (str == "json")   return ExportFormat::Json;
    if (str == "xml")   return ExportFormat::Xml;
    if (str == "csv")   return ExportFormat::Csv;
    if (str == "yaml")  return ExportFormat::Yaml;

    return ExportFormat::Unknown;
}

}

Q_DECLARE_METATYPE(Core::Tools::Exporter::ExportFormat)