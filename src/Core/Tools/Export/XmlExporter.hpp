#pragma once
#include "IRepositoryExporter.hpp"
#include "ExportOptions.hpp"
#include <QDomDocument>
#include <QDomElement>

namespace Core::Tools::Exporter
{

class XmlExporter : public IRepositoryExporter
{
public:

    XmlExporter(XmlExportOptions opts) : m_opts(std::move(opts)) {}

    QString exportData(const ExportRows &data) const override;

    QString fileExtension() const override;

    ExportFormat format() const noexcept override;


private:

    XmlExportOptions m_opts;

};

}