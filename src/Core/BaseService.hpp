#ifndef BASESERVICE_HPP
#define BASESERVICE_HPP
#include <QObject>
#include "Tools/Export/ExporterCreator.hpp"
#include "Tools/Export/Converters.hpp"

namespace Core
{

class BaseService : public QObject
{
    Q_OBJECT
public:
    explicit BaseService(QObject *parent = nullptr);

protected:

    template<typename BuiltFn, typename SendFn>
    void validateAndSend(const QVariantMap& params, BuiltFn&& build, SendFn&& send)
    {
        auto res = build(params);
        if (!res.has_value())
        {
            emit errorOccurred(res.error());
            return;
        }
        send(res.value());
    }

    template<typename TRow, typename ConverterFn>
    bool doExport(const QList<TRow> rows, Tools::Exporter::ExportFormat fmt,
                  const QString& path, ConverterFn&& conv, const QVariantMap& opts)
    {
        Tools::Exporter::ExportRows exportRows;
        exportRows.reserve(rows.size());
        for (const auto& row : rows)
            exportRows.append(conv(row));

        auto exporterRows = Tools::Exporter::ExporterCreator::create(fmt, opts);

        if (!exporterRows.has_value())
        {
            emit errorOccurred("export: " + exporterRows.error());
            return false;
        }

        if (!exporterRows.value()->saveToFile(exportRows, path))
        {
            emit errorOccurred("export: failed to write file \"" + path + "\"");
            return false;
        }

        emit messageReceived(QStringLiteral("Exported %1 row(s) to %2")
                                 .arg(exportRows.size()).arg(path));
        return true;
    }

    template<class T, class F>
    void postTo(T* obj, F&& f)
    {
        QMetaObject::invokeMethod(obj, std::forward<F>(f));
    }

signals:

    // Уведомляет об ошибке
    void errorOccurred(const QString& error);
    // Уведомляет об предупреждении
    void warningOccurred(const QString& warning);
    // Уведомляет об прогрессе загрузки запроса
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    // Уведомляет о старте потоковых данных
    void streamerStarted(const QString& id);
    // Уведомляет о остановке потоковых данных
    void streamerStopped(const QString& id);

    void messageReceived(const QString& msg);

};

}

#endif // BASESERVICE_HPP
