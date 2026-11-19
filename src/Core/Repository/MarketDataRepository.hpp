#pragma once
#include <QObject>
#include "Tools/Repository/PublicTradesRepository.hpp"
#include "Tools/Repository/KlinesRepository.hpp"

namespace Core::Repository
{

class MarketDataRepository : public QObject
{
    Q_OBJECT
private:

    std::unique_ptr<Tools::KlinesRepository> m_klinesRep;
    std::unique_ptr<Tools::PublicTradesRepository> m_publicTradesRep;
    mutable QMutex m_mutex;

public:

    explicit MarketDataRepository(QObject* parent = nullptr);
    void init(bool testnet);

    Klines loadKlines(EnumHelpers::MarketType type, const QString& symbol,
                             EnumHelpers::Interval interval,
                             std::optional<qint64> start,
                             std::optional<qint64> end);
    PublicTrades loadPublicTrades(EnumHelpers::MarketType type, const QString& symbol);

    void clearKlines();
    void clearPublicTrades();

    // Сохранение данных в крипторепозитории
    void saveKlines(const KlineInfo& newKline);
    void saveKlines(const Klines& newKlines);
    void savePublicTrades(const PublicTrades newTrades);

    Klines getKlines() const { return m_klinesRep->getData(); }
    PublicTrades getPublicTrades() const { return m_publicTradesRep->getData(); }

signals:

    void errorOccurred(const QString& error);

};

}

