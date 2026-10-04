#pragma once
#include "ExportOptions.hpp"
#include "IRepositoryExporter.hpp"

namespace Core::Tools::Exporter
{

class YamlExporter : public IRepositoryExporter
{
public:


    explicit YamlExporter(YamlExportOptions opts) : m_opts(std::move(opts)) {}

    QString exportData(const ExportRows& data) const override;

    QString fileExtension() const override;

    virtual ExportFormat format() const noexcept override;

private:

    YamlExportOptions m_opts;

    static QString renderScalar(const QVariant& value);
    static QString quoteStringIfNeeded(const QString& value);

};

}