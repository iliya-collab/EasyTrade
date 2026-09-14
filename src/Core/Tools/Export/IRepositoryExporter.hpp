#pragma once
#include <QString>

namespace Core::Tools::Exporter
{

template<typename TRow>
class IRepositoryExporter
{
public:

    virtual ~IRepositoryExporter() = default;

    // Сериализует контейнер в строку
    virtual QString exportData(const QList<TRow>& data) const = 0;

    // Расширение файла: "json", "xml", ...
    virtual QString fileExtension() const = 0;

    // Сохранить в файл
    virtual bool saveToFile(const QList<TRow>& data, const QString& path) const;

};

}