#pragma once
#include "ExportFormats.hpp"
#include "IRepositoryExporter.hpp"
#include "ExportOptionsBuilders.hpp"
#include "JsonExporter.hpp"
#include "XmlExporter.hpp"
#include "CsvExporter.hpp"
#include "YamlExporter.hpp"
#include <expected>

namespace Core::Tools::Exporter
{

class ExporterCreator
{
public:

    static std::expected< std::unique_ptr<IRepositoryExporter>, QString >
    create(ExportFormat fmt, const QVariantMap& opts = {})
    {
        switch (fmt)
        {

        case ExportFormat::Json:
        {
            auto o = buildJsonExportOptions(opts);
            if (!o.has_value())
                return std::unexpected(o.error());
            return std::make_unique<JsonExporter>(o.value());
        }

        case ExportFormat::Xml:
        {
            auto o = buildXmlExportOptions(opts);
            if (!o.has_value())
                return std::unexpected(o.error());
            return std::make_unique<XmlExporter>(o.value());
        }

        case ExportFormat::Csv:
        {
            auto o = buildCsvExportOptions(opts);
            if (!o.has_value())
                return std::unexpected(o.error());
            return std::make_unique<CsvExporter>(o.value());
        }

        case ExportFormat::Yaml:
        {
            auto o = buildYamlExportOptions(opts);
            if (!o.has_value())
                return std::unexpected(o.error());
            return std::make_unique<YamlExporter>(o.value());
        }

        default:
            return std::unexpected("export: unsupported format");

        }
    }

};

}
