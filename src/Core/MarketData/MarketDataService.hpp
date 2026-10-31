#pragma once
#include "MarketDataMediator.hpp"
#include "MarketDataState.hpp"
#include "Tools/Export/ExporterCreator.hpp"
#include "Tools/Export/Converters.hpp"

namespace Core {

class MarketDataService : public QObject {
    Q_OBJECT

private:

    std::shared_ptr<MarketDataState> m_state;
    std::shared_ptr<MarketDataMediator> m_mediator;

public:

    explicit MarketDataService(std::shared_ptr<MarketDataState> state, std::shared_ptr<MarketDataMediator> mediator, QObject *parent = nullptr);

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

    Q_INVOKABLE void run();
    Q_INVOKABLE void restart();
    Q_INVOKABLE void shutdown();
    Q_INVOKABLE void init(bool isTestnet);
    Q_INVOKABLE void subscribeSymbol(Tools::EnumHelpers::MarketType category, const QString& symbol);

    Q_INVOKABLE void loadInstruments(Core::Tools::EnumHelpers::MarketType type);
    Q_INVOKABLE void loadKlines(const QVariantMap& params);
    Q_INVOKABLE void exportKlines(const QString& path,
                                  Tools::Exporter::ExportFormat format,
                                  const QVariantMap& opts,
                                  const QVariantMap& params);

    Q_INVOKABLE void exportTrades(const QString& path,
                                  Tools::Exporter::ExportFormat format,
                                  const QVariantMap& opts,
                                  const QVariantMap& params);

private slots:

    void onErrorOccurredWithId(const QString& id, const QString& error);
    void onStreamerStarted(const QString& id);
    void onStreamerStopped(const QString& id);
    void onErrorOccurred(const QString& error);

signals:

    // Уведомляет об ошибке
    void errorOccurred(const QString& error);
    // Уведомляет об прогрессе загрузки запроса
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    // Уведомляет о старте потоковых данных
    void streamerStarted(const QString& id);
    // Уведомляет о остановке потоковых данных
    void streamerStopped(const QString& id);

    void messageReceived(const QString& msg);

};


}


