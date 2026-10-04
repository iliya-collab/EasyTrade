#pragma once
#include <QString>
#include <QChar>
#include <optional>

namespace Core::Tools::Exporter
{

// Параметры JsonExporter.
struct JsonExportOptions
{
    std::optional<QString> rootName;    // Опционально: пусто -> массив без обёртки
    bool    pretty = true;              // Indented vs Compact
};

// Параметры XmlExporter.
struct XmlExportOptions
{
    QString rootName = "items";         // Обязательно непусто — имя корневого элемента
    QString rowName  = "item";          // Обязательно непусто — имя элемента строки
    int     indent   = 4;               // Отступ в QDomDocument::toString()
};

// Параметры CsvExporter.
struct CsvExportOptions
{
    QChar delimiter  = ',';
    bool  withHeader = true;
};

// Параметры YamlExporter.
struct YamlExportOptions
{
    std::optional<QString> rootName;    // Опционально: пусто -> список без корневого ключа
    int     indentSize = 2;             // Обязательно > 0
};

}