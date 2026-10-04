#include "XmlExporter.hpp"

QString Core::Tools::Exporter::XmlExporter::exportData(const ExportRows &data) const
{
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""));

    QDomElement root = doc.createElement(m_opts.rootName);
    root.setAttribute("count", static_cast<int>(data.size()));
    doc.appendChild(root);

    for (const auto& row : data)
    {
        QDomElement rowEl = doc.createElement(m_opts.rowName);
        for (const auto& [key, value] : row)
        {
            QDomElement field = doc.createElement(key);
            field.appendChild(doc.createTextNode(value.toString()));
            rowEl.appendChild(field);
        }
        root.appendChild(rowEl);
    }

    return doc.toString(m_opts.indent);
}

QString Core::Tools::Exporter::XmlExporter::fileExtension() const { return exportFormatToString(format()); }

Core::Tools::Exporter::ExportFormat Core::Tools::Exporter::XmlExporter::format() const noexcept { return ExportFormat::Xml; }
