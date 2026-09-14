#pragma once
#include "ExportFormats.hpp"
#include "IRepositoryExporter.hpp"
#include "JsonExporter.hpp"
#include "XmlExporter.hpp"

namespace Core::Tools::Exporter
{

template <typename TRow>
class ExporterCreater
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
        default:
            return nullptr;
        }
    }

};

}
