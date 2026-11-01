#include "MarketDataState.hpp"
#include "Tools/Logging.hpp"

namespace Core {

    MarketDataState::MarketDataState(QObject *parent)
        : QObject{parent}
    {
        m_instruments = std::make_shared<Tools::InstrumentModel>();
        m_klineStore = std::make_shared<Tools::KlineStore>();
        m_asks = std::make_shared<Tools::OrderbookSideModel>(Core::Tools::OrderbookSideModel::Side::Ask);
        m_bids = std::make_shared<Tools::OrderbookSideModel>(Core::Tools::OrderbookSideModel::Side::Bid);
        m_trades = std::make_shared<Tools::PublicTradesModel>();
    }

    void MarketDataState::selectCurrentInstrument(const Tools::InstrumentInfo &instrument)
    {
        m_currentInstrument = instrument;
        emit currentInstrumentChanged();
    }

    void MarketDataState::updateInstruments(const Tools::Instruments& instruments)
    {
        m_instruments->updateInstruments(instruments);
        emit instrumentsChanged();
    }

    void MarketDataState::updateKline(const Tools::Kline &kline)
    {
        m_klineStore->updateKline(kline);
        emit klineStoreChanged();
    }

    void MarketDataState::addHistoricalKlines(const QList<Tools::Kline>& klines)
    {
        m_klineStore->addHistoricalKlines(klines);
    }

    void MarketDataState::updateTicker(const Tools::Ticker& ticker)
    {
        m_ticker = ticker;
        emit tickerChanged();
    }

    void MarketDataState::updateOrderbook(const Tools::Orderbook& orderbook)
    {
        m_asks->update(orderbook.m_asks);
        m_bids->update(orderbook.m_bids);
        emit asksChanged();
        emit bidsChanged();
    }

    void MarketDataState::updateTrades(const Tools::PublicTrades& trades)
    {
        m_trades->addTradeBatch(trades);
        emit tradesChanged();
    }

    void MarketDataState::updatePingMs(const QString& id, qint64 pingMs)
    {
        m_pingMs = pingMs;
        emit pingMsChanged();
    }

}

