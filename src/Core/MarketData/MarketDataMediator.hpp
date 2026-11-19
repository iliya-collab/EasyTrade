#pragma once
#include "MultiMediator.hpp"
#include "Core/ConnectionsPool.hpp"
#include "Service/IMarketDataService.hpp"
#include "Repository/MarketDataRepository.hpp"
#include "Tools/ExportStructures.hpp"

namespace Core {

class MarketDataMediator : public MultiMediator
{
    Q_OBJECT
private:

    std::unique_ptr<Service::IMarketDataService> m_publicService;
    std::unique_ptr<Repository::MarketDataRepository> m_marketDataRep;
    ConnectionsPool m_pool; // Хранит соединения для spot и linear рынков

public:

    explicit MarketDataMediator(QObject* parent = nullptr);

    void subscribe(EnumHelpers::MarketType market, const QString& symbol);
    void unsubscribe(EnumHelpers::MarketType market, const QString &symbol);

    Klines readKlinesFromRepository(const Tools::StdExport& std, const Tools::SpecialExportKlines& special);
    PublicTrades readPublicTradesFromRepository(const Tools::StdExport& std);

    void loadInstruments(EnumHelpers::MarketType type);
    void loadKlines(const Tools::KlinesRequest& req);

    void init(bool isTestnet);

    ConnectionsPool& pool() override;
    const ConnectionsPool& cpool() const override;

signals:

    // Уведомляет о получении данных
    void instrumentsReady(const Instruments& instruments);
    void tickerReady(const TickerInfo& ticker);
    void snapshotOrderbookReady(const OrderbookInfo& orderbook);
    void deltaUpdateOrderbookReady(const OrderbookInfo& orderbook);
    void klineUpdated(const KlineInfo& kline);
    void historicalKlinesReady(const Klines& klines);
    void tradesReady(const PublicTrades& trades);

private slots:

    void onInstrumentsReady(const Instruments& instruments);
    void onKlinesReady(const Klines& klines);
    void onKlineReceived(const KlineInfo& kline);
    void onPublicTradesReceived(const PublicTrades& trades);

};

}