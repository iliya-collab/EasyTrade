#pragma once
#include <QObject>
#include "Tools/Repository/InstrumentsRepository.hpp"
#include "Tools/Repository/PublicTradesRepository.hpp"
#include "Tools/Repository/KlinesRepository.hpp"

namespace Core::Repository
{

class MarketDataRepository : public QObject
{
    Q_OBJECT
private:

    std::unique_ptr<Tools::InstrumentsRepository> m_instrumentsRep;
    std::unique_ptr<Tools::KlinesRepository> m_klinesRep;
    std::unique_ptr<Tools::PublicTradesRepository> m_publicTradesRep;
    mutable QMutex m_mutex;

public:

    explicit MarketDataRepository(QObject* parent = nullptr);
    void init(bool testnet);

    Tools::Instruments loadInstruments(Tools::EnumHelpers::MarketType type);
    Tools::Klines loadKlines(Tools::EnumHelpers::MarketType type, const QString& symbol,
                             Tools::EnumHelpers::Interval interval,
                             std::optional<qint64> start,
                             std::optional<qint64> end);
    Tools::PublicTrades loadPublicTrades(Tools::EnumHelpers::MarketType type, const QString& symbol);

    void clearInstruments();
    void clearKlines();
    void clearPublicTrades();

    // Сохранение данных в крипторепозитории
    void saveInstruments(const Tools::Instruments& newInstruments);
    void saveKlines(const Tools::Kline& newKline);
    void saveKlines(const QList<Tools::Kline>& newKlines);
    void savePublicTrades(const Tools::PublicTrades newTrades);

    Tools::Instruments getInstruments() const { return m_instrumentsRep->getData(); }
    QList<Tools::Kline> getKlines() const { return m_klinesRep->getData(); }
    Tools::PublicTrades getPublicTrades() const { return m_publicTradesRep->getData(); }

signals:

    void errorOccurred(const QString& error);

};

}

