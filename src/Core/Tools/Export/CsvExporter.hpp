#pragma once
#include "IRepositoryExporter.hpp"
#include <QChar>
#include <QStringList>

namespace Core::Tools::Exporter
{

template<typename TRow>
class CsvExporter : public IRepositoryExporter<TRow>
{
public:

    using Converter = std::function<QList<QPair<QString, QVariant>>(const TRow&)>;

    explicit CsvExporter(Converter conv, QChar delimiter = ',', bool withHeader = true)
        : m_conv(std::move(conv)), m_delimiter(delimiter), m_withHeader(withHeader)
    {}

    QString exportData(const QList<TRow>& data) const override;

    QString fileExtension() const override;

    virtual ExportFormat format() const noexcept override;

private:

    Converter m_conv;
    QChar     m_delimiter;
    bool      m_withHeader;

    static QString escapeField(const QString& field, QChar delimiter);

};

}
