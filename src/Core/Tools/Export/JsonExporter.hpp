#pragma once
#include "IRepositoryExporter.hpp"
#include "ExportOptions.hpp"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

namespace Core::Tools::Exporter
{

class JsonExporter : public IRepositoryExporter
{
public:

    explicit JsonExporter(JsonExportOptions opts) : m_opts(std::move(opts)) { }

    QString exportData(const ExportRows &data) const override;
    /*{
        QJsonArray arr;
        for (const auto& row : data)
        {
            QJsonObject obj;
            for (const auto& [key, value] : m_conv(row))
                obj[key] = QJsonValue::fromVariant(value);
            arr.append(obj);

        }

        QJsonDocument doc;
        if (m_opts.rootName.has_value() && !m_opts.rootName.value().isEmpty())
            doc.setArray(arr);
        else
        {
            QJsonObject root;
            root[m_opts.rootName.value()] = arr;
            root["count"] = arr.size();
            doc.setObject(root);
        }

        return QString::fromUtf8(doc.toJson(m_opts.pretty ? QJsonDocument::Indented : QJsonDocument::Compact));
    }*/

    QString fileExtension() const override;

    ExportFormat format() const noexcept override;

private:

    JsonExportOptions m_opts;

};

}