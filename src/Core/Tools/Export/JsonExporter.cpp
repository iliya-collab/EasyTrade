#include "JsonExporter.hpp"

QString Core::Tools::Exporter::JsonExporter::exportData(const ExportRows &data) const
{
    QJsonArray arr;
    for (const auto& row : data)
    {
        QJsonObject obj;
        for (const auto& [key, value] : row)
            obj[key] = QJsonValue::fromVariant(value);
        arr.append(obj);

    }

    QJsonDocument doc;
    if (!m_opts.rootName.has_value() || m_opts.rootName->isEmpty())
    {
        doc.setArray(arr);
    }
    else
    {
        QJsonObject root;
        root[m_opts.rootName.value()] = arr;
        root["count"] = arr.size();
        doc.setObject(root);
    }

    return QString::fromUtf8(doc.toJson(m_opts.pretty ? QJsonDocument::Indented : QJsonDocument::Compact));
}

QString Core::Tools::Exporter::JsonExporter::fileExtension() const { return exportFormatToString(format()); }

Core::Tools::Exporter::ExportFormat Core::Tools::Exporter::JsonExporter::format() const noexcept { return ExportFormat::Json; }
