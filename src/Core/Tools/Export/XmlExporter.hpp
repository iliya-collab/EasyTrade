// Tools/Export/XmlExporter.hpp
#pragma once
#include "IRepositoryExporter.hpp"
#include <QDomDocument>
#include <QDomElement>

namespace Core::Tools::Exporter
{

template <typename TRow>
class XmlExporter : public IRepositoryExporter<TRow>
{
public:

    using Converter = std::function<QList<QPair<QString, QString>>(const TRow&)>;

    XmlExporter(Converter conv, QString rootName = "items", QString rowName = "item", int indent = 4)
        : m_conv(std::move(conv)), m_rootName(std::move(rootName)), m_rowName(std::move(rowName)), m_indent(indent)
    {}

    QString exportData(const QList<TRow>& data) const override;

    QString fileExtension() const override;

private:

    Converter m_conv;
    QString   m_rootName;
    QString   m_rowName;
    int       m_indent;

};

}