#pragma once
#include "ExportOptions.hpp"
#include <QVariantMap>
#include <expected>

namespace Core::Tools::Exporter
{

    namespace detail
    {

    inline std::optional<QString> optString(const QVariantMap& map, const QString& key)
    {
        auto it = map.constFind(key);
        if (it == map.constEnd())
            return std::nullopt;
        return it->toString();
    }

    }

inline std::expected<JsonExportOptions, QString> buildJsonExportOptions(const QVariantMap& params)
{
    JsonExportOptions opts;
    opts.rootName = detail::optString(params, "rootName");
    opts.pretty   = params.value("pretty", true).toBool();

    return opts;
}

inline std::expected<XmlExportOptions, QString> buildXmlExportOptions(const QVariantMap& params)
{
    XmlExportOptions opts;
    if (params.contains("rootName"))
        opts.rootName = params.value("rootName").toString();
    if (params.contains("rowName"))
        opts.rowName = params.value("rowName").toString();
    opts.indent = params.value("indent", 4).toInt();

    if (opts.rootName.isEmpty())
        return std::unexpected<QString>("xml export: 'rootName' must not be empty");
    if (opts.rowName.isEmpty())
        return std::unexpected<QString>("xml export: 'rowName' must not be empty");
    if (opts.indent < 0)
        return std::unexpected<QString>("xml export: 'indent' must be >= 0");

    return opts;
}

inline std::expected<CsvExportOptions, QString> buildCsvExportOptions(const QVariantMap& params)
{
    CsvExportOptions opts;

    const QString delimiterStr = params.value("delimiter", ",").toString();
    if (delimiterStr.length() != 1)
        return std::unexpected<QString>("csv export: 'delimiter' must be exactly one character");

    opts.delimiter  = delimiterStr.at(0);
    opts.withHeader = params.value("withHeader", true).toBool();

    return opts;
}

inline std::expected<YamlExportOptions, QString> buildYamlExportOptions(const QVariantMap& params)
{
    YamlExportOptions opts;
    opts.rootName   = detail::optString(params, "rootName");
    opts.indentSize = params.value("indentSize", 2).toInt();

    if (opts.indentSize <= 0)
        return std::unexpected<QString>("yaml export: 'indentSize' must be > 0");

    return opts;
}

}