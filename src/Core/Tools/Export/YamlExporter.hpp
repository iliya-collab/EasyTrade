#pragma once
#include "IRepositoryExporter.hpp"

namespace Core::Tools::Exporter
{

template <typename TRow>
class YamlExporter : public IRepositoryExporter<TRow>
{
public:

    using Converter = std::function<QList<QPair<QString, QVariant>>(const TRow&)>;

    explicit YamlExporter(Converter conv, QString rootName = {}, int indentSize = 2)
        : m_conv(std::move(conv)), m_rootName(std::move(rootName)), m_indentSize(indentSize)
    {}

    QString exportData(const QList<TRow>& data) const override;

    QString fileExtension() const override;

    virtual ExportFormat format() const noexcept override;

private:

    Converter m_conv;
    QString   m_rootName;
    int       m_indentSize;

    static QString renderScalar(const QVariant& value);
    static QString quoteStringIfNeeded(const QString& value);

};

}