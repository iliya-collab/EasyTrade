#pragma once
#include "MarketDataMediator.hpp"
#include "MarketDataState.hpp"
#include "Core/BaseService.hpp"

namespace Core {

class MarketDataService : public BaseService {
    Q_OBJECT

private:

    std::shared_ptr<MarketDataState> m_state;
    std::shared_ptr<MarketDataMediator> m_mediator;

public:

    explicit MarketDataService(std::shared_ptr<MarketDataState> state, std::shared_ptr<MarketDataMediator> mediator, QObject *parent = nullptr);

    Q_INVOKABLE void run();
    Q_INVOKABLE void restart();
    Q_INVOKABLE void shutdown();
    Q_INVOKABLE void init(bool isTestnet);

    Q_INVOKABLE void openInstrument(const InstrumentInfo& inst);                // ensure + subscribe
    Q_INVOKABLE void closeInstrument(const InstrumentInfo& inst);               // unsubscribe + remove
    Q_INVOKABLE void selectCurrentInstrument(const InstrumentInfo& inst);       // только m_state->setCurrent

    Q_INVOKABLE void loadInstruments(EnumHelpers::MarketType type);
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


};


}


