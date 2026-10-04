#pragma once
#include "Tools/DataModels/TradePairsModel.hpp"
#include "Tools/DataModels/KlineSeriesModel.hpp"
#include "Tools/DataModels/OrderbookSideModel.hpp"
#include "Tools/DataModels/PublicTradesModel.hpp"
#include "Tools/DataModels/ReversedProxyModel.hpp"
#include "Tools/DataModels/TradePairsFilterProxyModel.hpp"
#include <QObject>

namespace Core {

    class MarketDataState : public QObject
    {
        Q_OBJECT

        // Readonly свойства
        Q_PROPERTY(QString                          currentSymbol   READ getCurrentSymbol   NOTIFY currentSymbolChanged FINAL)
        Q_PROPERTY(Core::Tools::TradePairsModel*    tradePairs      READ getTradePairs      NOTIFY tradePairsChanged    FINAL)
        Q_PROPERTY(Core::Tools::KlineStore*         klineStore      READ getKlineStore      NOTIFY klineStoreChanged    FINAL)
        Q_PROPERTY(qint64                           pingMs          READ getPingMs          NOTIFY pingMsChanged        FINAL)
        Q_PROPERTY(Core::Tools::Ticker              ticker          READ getTicker          NOTIFY tickerChanged        FINAL)
        Q_PROPERTY(Core::Tools::OrderbookSideModel* asks            READ getAsks            NOTIFY asksChanged          FINAL)
        Q_PROPERTY(Core::Tools::OrderbookSideModel* bids            READ getBids            NOTIFY bidsChanged          FINAL)
        Q_PROPERTY(Core::Tools::PublicTradesModel*  trades          READ getTrades          NOTIFY tradesChanged        FINAL)

    private:

        // Рыночные данные
        QString m_currentSymbol{};
        Tools::Ticker m_ticker{};
        std::shared_ptr<Tools::KlineStore> m_klineStore{};
        std::shared_ptr<Tools::OrderbookSideModel> m_asks{};
        std::shared_ptr<Tools::OrderbookSideModel> m_bids{};
        std::shared_ptr<Tools::PublicTradesModel> m_trades{};
        // Прочее
        std::shared_ptr<Tools::TradePairsModel> m_tradePairs{};
        qint64 m_pingMs = 0;

    public:

        explicit MarketDataState(QObject *parent = nullptr);

        // Методы обновления
        void setCurrentSymbol(const QString& symbol);
        void updateTradePairs(const QList<Tools::TradeInfo>& pairs);
        void updateKline(const Tools::Kline& kline);
        void addHistoricalKlines(const QList<Tools::Kline>& klines);
        void updateTicker(const Tools::Ticker& ticker);
        void updateOrderbook(const Tools::Orderbook& orderbook);
        void updateTrades(const Tools::PublicTrades& trades);
        void updatePingMs(qint64 pingMs);

        // READ-методы
        QString getCurrentSymbol() const { return m_currentSymbol; }
        Tools::KlineStore* getKlineStore() const { return m_klineStore.get(); }
        Tools::Ticker getTicker() const { return m_ticker; }
        Tools::OrderbookSideModel* getAsks() const { return m_asks.get(); }
        Tools::OrderbookSideModel* getBids() const { return m_bids.get(); }
        Tools::PublicTradesModel* getTrades() const { return m_trades.get(); }
        Tools::TradePairsModel* getTradePairs() const { return m_tradePairs.get(); }
        qint64 getPingMs() const { return m_pingMs; }

    signals:

        void currentSymbolChanged();
        void tradePairsChanged();
        void klineStoreChanged();
        void tickerChanged();
        void asksChanged();
        void bidsChanged();
        void pingMsChanged();
        void tradesChanged();

    };

}
