#pragma once
#include "IRepositoryExporter.hpp"
#include "ExportOptions.hpp"
#include <QChar>
#include <QStringList>

namespace Core::Tools::Exporter
{

class CsvExporter : public IRepositoryExporter
{
public:


    explicit CsvExporter(CsvExportOptions opts) : m_opts(std::move(opts)) {}

    QString exportData(const ExportRows& data) const override;

    QString fileExtension() const override;

    virtual ExportFormat format() const noexcept override;

private:

    CsvExportOptions m_opts;

    static QString escapeField(const QString& field, QChar delimiter);

};

}
