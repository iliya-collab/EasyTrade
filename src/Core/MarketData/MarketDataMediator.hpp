#pragma once
#include "BaseMediator.hpp"
#include "Service/IMarketDataService.hpp"
#include "Streamer/IMarketDataStreamer.hpp"
#include "Repository/MarketDataRepository.hpp"
#include "Tools/ExportStructures.hpp"

namespace Core {

class MarketDataMediator : public BaseMediator
{
    Q_OBJECT
private:

    std::unique_ptr<Service::IMarketDataService> m_publicService;
    std::unique_ptr<Streamer::IMarketDataStreamer> m_publicStreamer;
    std::unique_ptr<Repository::MarketDataRepository> m_marketDataRep;

public:

    explicit MarketDataMediator(QObject* parent = nullptr);

    void subscribe(const QString& symbol);

    Tools::Instruments readInstrumentsFromRepository(Tools::EnumHelpers::MarketType type);
    QList<Tools::Kline> readKlinesFromRepository(const Tools::StdExport& std, const Tools::SpecialExportKlines& special);
    Tools::PublicTrades readPublicTradesFromRepository(const Tools::StdExport& std);

    void loadInstrumentsFromNetwork(Tools::EnumHelpers::MarketType type);
    void loadKlinesFromNetwork(const Tools::KlinesRequest& req);

    void init(bool isTestnet);

protected:

    Streamer::IStreamer* streamer() override { return m_publicStreamer.get(); }

signals:

    // Уведомляет о получении данных
    void instrumentsReady(const Core::Tools::Instruments& instruments);
    void tickerReady(const Core::Tools::Ticker& ticker);
    void orderbookReady(const Core::Tools::Orderbook& orderbook);
    void klineUpdated(const Core::Tools::Kline& kline);
    void historicalKlinesReady(const QList<Core::Tools::Kline>& klines);
    void tradesReady(const Core::Tools::PublicTrades& trades);

private slots:

    void onInstrumentsReady(const Core::Tools::Instruments& instruments);
    void onKlinesReady(const QList<Core::Tools::Kline>& klines);
    void onKlineReceived(const Core::Tools::Kline& kline);
    void onPublicTradesReceived(const Core::Tools::PublicTrades& trades);

};

}