#pragma once
#include <QObject>
#include "Tools/Repository/PublicTradesRepository.hpp"
#include "Tools/Repository/KlinesRepository.hpp"
#include "Tools/Repository/CryptoRepository.hpp"

namespace Core::Markets
{

class MarketDataRepository : public QObject
{
    Q_OBJECT
private:

    std::unique_ptr<Tools::CryptoRepository> m_cryptoRep;
    std::unique_ptr<Tools::KlinesRepository> m_klinesRep;
    std::unique_ptr<Tools::PublicTradesRepository> m_publicTradesRep;
    mutable QMutex m_mutex;

public:

    explicit MarketDataRepository(QObject* parent = nullptr);

    // Загрузка всех данных из крипторепозитория
    QList<Tools::TradeInfo> loadFromCryptoRepository(Tools::MarketType type);
    QList<Tools::Kline> loadFromKlinesRepository(Tools::MarketType type, const QString& symbol, Tools::Interval interval, std::optional<qint64> start, std::optional<qint64> end);
    Tools::PublicTrades loadFromPublicTradesRepository(Tools::MarketType type, const QString& symbol);

    void clearCryptoRepository();
    void clearKlinesRepository();
    void clearPublicTradesRepository();

    // Сохранение данных в крипторепозитории
    void saveToCryptoRepository(const QList<Tools::TradeInfo>& tradePairs);
    void saveToKlinesRepository(const Tools::Kline& newKline);
    void saveToKlinesRepository(const QList<Tools::Kline>& newKlines);
    void saveToPublicTradesRepository(const Tools::PublicTrades newTrades);

    QList<Tools::TradeInfo> getTradeList() { return m_cryptoRep->getData(); }
    QList<Tools::Kline> getKlines() { return m_klinesRep->getData(); }
    Tools::PublicTrades getPublicTrades() { return m_publicTradesRep->getData(); }

signals:

    void errorOccurred(const QString& error);

};

}

