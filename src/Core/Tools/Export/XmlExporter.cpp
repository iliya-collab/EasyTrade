#include "XmlExporter.hpp"

template<typename TRow>
QString Core::Tools::Exporter::XmlExporter<TRow>::exportData(const QList<TRow> &data) const
{
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""));

    QDomElement root = doc.createElement(m_rootName);
    root.setAttribute("count", static_cast<int>(data.size()));
    doc.appendChild(root);

    for (const auto& row : data)
    {
        QDomElement rowEl = doc.createElement(m_rowName);
        for (const auto& [key, value] : m_conv(row))
        {
            QDomElement field = doc.createElement(key);
            field.appendChild(doc.createTextNode(value.toString()));
            rowEl.appendChild(field);
        }
        root.appendChild(rowEl);
    }

    return doc.toString(m_indent);
}

template<typename TRow>
QString Core::Tools::Exporter::XmlExporter<TRow>::fileExtension() const { return exportFormatToString(format()); }

template<typename TRow>
Core::Tools::Exporter::ExportFormat Core::Tools::Exporter::XmlExporter<TRow>::format() const noexcept
{ return ExportFormat::Xml; }
