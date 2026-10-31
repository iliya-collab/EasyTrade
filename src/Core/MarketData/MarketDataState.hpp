#pragma once
#include "Tools/DataModels/InstrumentModel.hpp"
#include "Tools/DataModels/KlineStore.hpp"
#include "Tools/DataModels/OrderbookSideModel.hpp"
#include "Tools/DataModels/PublicTradesModel.hpp"
#include <QObject>

namespace Core {

    class MarketDataState : public QObject
    {
        Q_OBJECT

        // Readonly свойства
        Q_PROPERTY(Tools::EnumHelpers::MarketType   currentCategory READ getCurrentCategory NOTIFY currentCategoryChanged   FINAL)
        Q_PROPERTY(QString                          currentSymbol   READ getCurrentSymbol   NOTIFY currentSymbolChanged     FINAL)
        Q_PROPERTY(Core::Tools::InstrumentModel*    instruments     READ getInstruments     NOTIFY instrumentsChanged       FINAL)
        Q_PROPERTY(Core::Tools::KlineStore*         klineStore      READ getKlineStore      NOTIFY klineStoreChanged        FINAL)
        Q_PROPERTY(qint64                           pingMs          READ getPingMs          NOTIFY pingMsChanged            FINAL)
        Q_PROPERTY(Core::Tools::Ticker              ticker          READ getTicker          NOTIFY tickerChanged            FINAL)
        Q_PROPERTY(Core::Tools::OrderbookSideModel* asks            READ getAsks            NOTIFY asksChanged              FINAL)
        Q_PROPERTY(Core::Tools::OrderbookSideModel* bids            READ getBids            NOTIFY bidsChanged              FINAL)
        Q_PROPERTY(Core::Tools::PublicTradesModel*  trades          READ getTrades          NOTIFY tradesChanged            FINAL)

    private:

        // Рыночные данные
        Tools::EnumHelpers::MarketType m_currentCategory;
        QString m_currentSymbol{};
        Tools::Ticker m_ticker{};
        std::shared_ptr<Tools::KlineStore> m_klineStore{};
        std::shared_ptr<Tools::OrderbookSideModel> m_asks{};
        std::shared_ptr<Tools::OrderbookSideModel> m_bids{};
        std::shared_ptr<Tools::PublicTradesModel> m_trades{};
        // Прочее
        std::shared_ptr<Tools::InstrumentModel> m_instruments{};
        qint64 m_pingMs = 0;

    public:

        explicit MarketDataState(QObject *parent = nullptr);

        // Методы обновления
        void setCurrentCategory(Tools::EnumHelpers::MarketType category);
        void setCurrentSymbol(const QString& symbol);
        void updateInstruments(const Core::Tools::Instruments& instruments);
        void updateKline(const Tools::Kline& kline);
        void addHistoricalKlines(const QList<Tools::Kline>& klines);
        void updateTicker(const Tools::Ticker& ticker);
        void updateOrderbook(const Tools::Orderbook& orderbook);
        void updateTrades(const Tools::PublicTrades& trades);
        void updatePingMs(const QString& id, qint64 pingMs);

        // READ-методы
        Tools::EnumHelpers::MarketType getCurrentCategory() const { return m_currentCategory; }
        QString getCurrentSymbol() const { return m_currentSymbol; }
        Tools::KlineStore* getKlineStore() const { return m_klineStore.get(); }
        Tools::Ticker getTicker() const { return m_ticker; }
        Tools::OrderbookSideModel* getAsks() const { return m_asks.get(); }
        Tools::OrderbookSideModel* getBids() const { return m_bids.get(); }
        Tools::PublicTradesModel* getTrades() const { return m_trades.get(); }
        Tools::InstrumentModel* getInstruments() const { return m_instruments.get(); }
        qint64 getPingMs() const { return m_pingMs; }

    signals:

        void currentCategoryChanged();
        void currentSymbolChanged();
        void instrumentsChanged();
        void klineStoreChanged();
        void tickerChanged();
        void asksChanged();
        void bidsChanged();
        void pingMsChanged();
        void tradesChanged();

    };

}
