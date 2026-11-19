#ifndef INSTRUMENTDATA_HPP
#define INSTRUMENTDATA_HPP
#include "Tools/DataModels/KlineStore.hpp"
#include "Tools/DataModels/OrderbookSideModel.hpp"
#include "Tools/DataModels/PublicTradesModel.hpp"
#include <QObject>

namespace Core {

class InstrumentData : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Owned by MarketDataState")

    // Readonly свойства
    Q_PROPERTY(InstrumentInfo      info       READ getInfo       NOTIFY infoChanged       FINAL)
    Q_PROPERTY(TickerInfo          ticker     READ getTicker     NOTIFY tickerChanged     FINAL)
    Q_PROPERTY(OrderbookSideModel* asks       READ getAsks       NOTIFY asksChanged       FINAL)
    Q_PROPERTY(OrderbookSideModel* bids       READ getBids       NOTIFY bidsChanged       FINAL)
    Q_PROPERTY(PublicTradesModel*  trades     READ getTrades     NOTIFY tradesChanged     FINAL)
    Q_PROPERTY(KlineStore*         klineStore READ getKlineStore NOTIFY klineStoreChanged FINAL)

public:

    explicit InstrumentData(const InstrumentInfo& info, QObject* parent);

    void updateTicker(const TickerInfo&);
    void snapshotOrderbook(const OrderbookInfo&);
    void deltaUpdateOrderbook(const OrderbookInfo&);
    void updateTrades(const PublicTrades&);
    void updateKline(const KlineInfo&);
    void addHistoricalKlines(const Klines& klines);

    // Read-методы
    [[nodiscard]] const InstrumentInfo& getInfo() const noexcept { return m_info; }
    [[nodiscard]] const TickerInfo&     getTicker() const noexcept { return m_ticker; }
    [[nodiscard]] OrderbookSideModel*   getAsks() noexcept { return &m_asks; }
    [[nodiscard]] OrderbookSideModel*   getBids() noexcept { return &m_bids; }
    [[nodiscard]] PublicTradesModel*    getTrades() noexcept { return &m_trades; }
    [[nodiscard]] KlineStore*           getKlineStore() noexcept { return &m_klineStore; }

private:

    InstrumentInfo m_info;
    TickerInfo m_ticker;
    OrderbookSideModel m_asks{Tools::OrderbookSideModel::Ask, this};
    OrderbookSideModel m_bids{Tools::OrderbookSideModel::Bid, this};
    PublicTradesModel m_trades{this};
    KlineStore m_klineStore{this};

signals:

    void infoChanged();
    void feeChanged();
    void klineStoreChanged();
    void tickerChanged();
    void asksChanged();
    void bidsChanged();
    void tradesChanged();

};

}

#endif // INSTRUMENTDATA_HPP
