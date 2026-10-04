#include "CsvExporter.hpp"

QString Core::Tools::Exporter::CsvExporter::escapeField(const QString& field, QChar delimiter)
{
    const bool needsQuoting = field.contains(delimiter)
        || field.contains('"')
        || field.contains('\n')
        || field.contains('\r');

    if (!needsQuoting)
        return field;

    QString escaped = field;
    escaped.replace('"', "\"\"");
    return '"' + escaped + '"';
}

QString Core::Tools::Exporter::CsvExporter::exportData(const ExportRows& data) const
{
    if (data.isEmpty())
        return QString();

    QString out;
    out.reserve(data.size() * 64);

    if (m_opts.withHeader)
    {
        QStringList headerCells;
        for (const auto& [key, value] : data.first())
            headerCells << escapeField(key, m_opts.delimiter);
        out += headerCells.join(m_opts.delimiter) + "\r\n";
    }

    for (const auto& row : data)
    {
        QStringList cells;
        for (const auto& [key, value] : row)
            cells << escapeField(value.toString(), m_opts.delimiter);
        out += cells.join(m_opts.delimiter) + "\r\n";
    }

    return out;
}

QString Core::Tools::Exporter::CsvExporter::fileExtension() const { return exportFormatToString(format()); }

Core::Tools::Exporter::ExportFormat Core::Tools::Exporter::CsvExporter::format() const noexcept { return ExportFormat::Csv; }