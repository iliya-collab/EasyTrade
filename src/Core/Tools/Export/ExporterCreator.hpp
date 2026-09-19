#pragma once
#include "ExportFormats.hpp"
#include "IRepositoryExporter.hpp"
#include "JsonExporter.hpp"
#include "XmlExporter.hpp"
#include "CsvExporter.hpp"
#include "YamlExporter.hpp"

namespace Core::Tools::Exporter
{

template <typename TRow>
class ExporterCreator
{
public:

    template<typename... Args>
    static std::unique_ptr<IRepositoryExporter<TRow>>
    create(ExportFormat fmt, Args&&... args)
    {
        switch (fmt)
        {
        case ExportFormat::Json:
            return std::make_unique<JsonExporter<TRow>>(std::forward<Args>(args)...);
        case ExportFormat::Xml:
            return std::make_unique<XmlExporter<TRow>>(std::forward<Args>(args)...);
        case ExportFormat::Csv:
            return std::make_unique<CsvExporter<TRow>>(std::forward<Args>(args)...);
        case ExportFormat::Yaml:
            return std::make_unique<YamlExporter<TRow>>(std::forward<Args>(args)...);
        default:
            return nullptr;
        }
    }

};

}
