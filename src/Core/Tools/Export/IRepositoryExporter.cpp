#include "IRepositoryExporter.hpp"
#include <QFile>
#include <QTextStream>

namespace Core::Tools::Exporter
{

template <typename TRow>
bool IRepositoryExporter<TRow>::saveToFile(const QList<TRow>& data, const QString& path) const
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&f);
    out << exportData(data);
    return true;
}

}
