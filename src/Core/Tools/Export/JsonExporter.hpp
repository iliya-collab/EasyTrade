#pragma once
#include "IRepositoryExporter.hpp"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

namespace Core::Tools::Exporter
{

template<typename TRow>
class JsonExporter : public IRepositoryExporter<TRow>
{
public:

    using Converter = std::function<QJsonObject(const TRow&)>;

    explicit JsonExporter(Converter conv, bool pretty = true, QString rootName = {})
        : m_conv(std::move(conv)), m_pretty(pretty), m_rootName(std::move(rootName))
    {}

    QString exportData(const QList<TRow>& data) const override;

    QString fileExtention() const override;

private:

    Converter m_conv;
    bool m_pretty;
    QString m_rootName;

};

}