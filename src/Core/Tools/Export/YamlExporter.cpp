#include "YamlExporter.hpp"
#include <QRegularExpression>
#include <QMetaType>
#include <cmath>

template<typename TRow>
QString Core::Tools::Exporter::YamlExporter<TRow>::quoteStringIfNeeded(const QString& value)
{
    static const QRegularExpression looksLikeNumber(R"(^[+-]?(\d+\.?\d*|\.\d+)([eE][+-]?\d+)?$)");
    static const QStringList reservedWords = {
        "null", "Null", "NULL", "~",
        "true", "True", "TRUE", "false", "False", "FALSE",
        "yes", "Yes", "YES", "no", "No", "NO",
        "on", "On", "ON", "off", "Off", "OFF"
    };

    const bool needsQuoting =
        value.isEmpty()
        || value != value.trimmed()
        || value.contains('\n') || value.contains('\r')
        || value.contains(": ") || value.endsWith(':')
        || value.contains(" #")
        || reservedWords.contains(value)
        || looksLikeNumber.match(value).hasMatch()
        || QString("-?:,[]{}#&*!|>'\"%@`").contains(value.at(0));

    if (!needsQuoting)
        return value;

    QString escaped = value;
    escaped.replace('\\', "\\\\");
    escaped.replace('"', "\\\"");
    return '"' + escaped + '"';
}

template<typename TRow>
QString Core::Tools::Exporter::YamlExporter<TRow>::renderScalar(const QVariant& value)
{
    if (!value.isValid() || value.isNull())
        return "null";

    switch (value.typeId())
    {
    case QMetaType::Bool:
        return value.toBool() ? "true" : "false";

    case QMetaType::Int:
    case QMetaType::UInt:
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        return value.toString();

    case QMetaType::Double:
    case QMetaType::Float:
    {
        const double d = value.toDouble();
        if (!std::isfinite(d))
            return quoteStringIfNeeded(QString::number(d));

        return QString::number(d, 'g', 17);
    }

    default:
        return quoteStringIfNeeded(value.toString());
    }
}

template<typename TRow>
QString Core::Tools::Exporter::YamlExporter<TRow>::exportData(const QList<TRow>& data) const
{
    QString out;
    const QString itemIndent(m_indentSize, ' ');
    const QString fieldIndent(m_indentSize * 2, ' ');

    if (data.isEmpty())
    {
        if (m_rootName.isEmpty())
            return "[]\n";
        return m_rootName + ": []\n";
    }

    if (!m_rootName.isEmpty())
        out += m_rootName + ":\n";

    for (const auto& row : data)
    {
        const auto fields = m_conv(row);
        if (fields.isEmpty())
        {
            out += itemIndent + "- {}\n";
            continue;
        }

        bool first = true;
        for (const auto& [key, value] : fields)
        {
            const QString rendered = renderScalar(value);

            if (first)
            {
                out += itemIndent + "- " + key + ": " + rendered + "\n";
                first = false;
            }
            else
            {
                out += fieldIndent + key + ": " + rendered + "\n";
            }
        }
    }

    return out;
}

template<typename TRow>
QString Core::Tools::Exporter::YamlExporter<TRow>::fileExtension() const { return exportFormatToString(format()); }

template<typename TRow>
Core::Tools::Exporter::ExportFormat Core::Tools::Exporter::YamlExporter<TRow>::format() const noexcept
{ return ExportFormat::Yaml; }