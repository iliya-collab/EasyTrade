#include "CsvExporter.hpp"

template<typename TRow>
QString Core::Tools::Exporter::CsvExporter<TRow>::escapeField(const QString& field, QChar delimiter)
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

template<typename TRow>
QString Core::Tools::Exporter::CsvExporter<TRow>::exportData(const QList<TRow>& data) const
{
    if (data.isEmpty())
        return QString();

    QString out;
    out.reserve(data.size() * 64);

    if (m_withHeader)
    {
        QStringList headerCells;
        for (const auto& [key, value] : m_conv(data.first()))
            headerCells << escapeField(key, m_delimiter);
        out += headerCells.join(m_delimiter) + "\r\n";
    }

    for (const auto& row : data)
    {
        QStringList cells;
        for (const auto& [key, value] : m_conv(row))
            cells << escapeField(value.toString(), m_delimiter);
        out += cells.join(m_delimiter) + "\r\n";
    }

    return out;
}

template<typename TRow>
QString Core::Tools::Exporter::CsvExporter<TRow>::fileExtension() const
{ return exportFormatToString(format()); }

template<typename TRow>
Core::Tools::Exporter::ExportFormat Core::Tools::Exporter::CsvExporter<TRow>::format() const noexcept
{ return ExportFormat::Csv; }