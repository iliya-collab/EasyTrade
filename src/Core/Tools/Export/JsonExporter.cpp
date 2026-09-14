#include "JsonExporter.hpp"

template<typename TRow>
QString Core::Tools::Exporter::JsonExporter<TRow>::exportData(const QList<TRow> &data) const
{
    QJsonArray arr;
    for (const auto& row : data)
        arr.append(m_conv(row));

    QJsonDocument doc;
    if (m_rootName.isEmpty())
        doc.setArray(arr);
    else
    {
        QJsonObject root;
        root[m_rootName] = arr;
        root["count"] = arr.size();
        doc.setObject(root);
    }

    return QString::fromUtf8(doc.toJson(m_pretty ? QJsonDocument::Indented : QJsonDocument::Compact));
}

template<typename TRow>
QString Core::Tools::Exporter::JsonExporter<TRow>::fileExtention() const { return "json"; }