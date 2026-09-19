#pragma once
#include <QString>
#include <QFile>
#include <QTextStream>
#include "ExportFormats.hpp"

namespace Core::Tools::Exporter
{

template<typename TRow>
class IRepositoryExporter
{
public:

    virtual ~IRepositoryExporter() = default;

    virtual ExportFormat format() const noexcept = 0;

    // Сериализует контейнер в строку
    virtual QString exportData(const QList<TRow>& data) const = 0;

    // Расширение файла: "json", "xml", ...
    virtual QString fileExtension() const = 0;

    // Сохранить в файл
    virtual bool saveToFile(const QList<TRow>& data, const QString& path) const
    {
        QFile f(path);
        if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
            return false;

        QTextStream out(&f);
        out << exportData(data);
        return true;
    }

};

}