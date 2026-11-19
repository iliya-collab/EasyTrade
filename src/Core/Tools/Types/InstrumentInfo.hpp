#ifndef INSTRUMENTINFO_HPP
#define INSTRUMENTINFO_HPP
#include "EnumHelpers.hpp"

namespace Core::Tools::Types
{

// ==========================================================================================
//  Комиссии по инструменту
// ==========================================================================================

struct FeeRatesInfo
{
    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown;
    QString m_symbol{};
    // Ордер сразу находит встречное предложение и закрывается
    QString m_takerFeeRate{};
    // Ордер не может исполниться прямо сейчас, потому что на рынке нет подходящей цены
    QString m_makerFeeRate{};
};

// ==========================================================================================
//  Инструмент
// ==========================================================================================

struct InstrumentInfo
{
    Q_GADGET
    QML_VALUE_TYPE(tInstrumentInfo)

    // Базовые свойства
    Q_PROPERTY(QString                   id          READ id          CONSTANT FINAL)
    Q_PROPERTY(EnumHelpers::MarketType   category    READ category    CONSTANT FINAL)
    Q_PROPERTY(QString                   symbol      READ symbol      CONSTANT FINAL)
    Q_PROPERTY(QString                   baseCoin    READ baseCoin    CONSTANT FINAL)
    Q_PROPERTY(QString                   quoteCoin   READ quoteCoin   CONSTANT FINAL)
    Q_PROPERTY(QString                   status      READ status      CONSTANT FINAL)
    Q_PROPERTY(QString                   tickSize    READ tickSize    CONSTANT FINAL) // Шаг изменения цены
    Q_PROPERTY(QString                   qtyStep     READ qtyStep     CONSTANT FINAL) // Шаг изменения количества
    Q_PROPERTY(QString                   minOrderQty READ minOrderQty CONSTANT FINAL) // Минимальное кол-во в ордере
    Q_PROPERTY(QString                   maxOrderQty READ maxOrderQty CONSTANT FINAL) // Максимальное кол-во (Лимит)
    Q_PROPERTY(QString                   maxMktQty   READ maxMktQty   CONSTANT FINAL) // Максимальное кол-во (Маркет)
    Q_PROPERTY(QString                   minNotional READ minNotional CONSTANT FINAL) // Минимальная стоимость ордера (Номинал)
    Q_PROPERTY(QString                   maxLeverage READ maxLeverage CONSTANT FINAL) // Максимальное плечо

public:
    // Геттеры
    EnumHelpers::MarketType category() const { return m_category; }
    QString symbol() const { return m_symbol; }
    QString baseCoin() const { return m_baseCoin; }
    QString quoteCoin() const { return m_quoteCoin; }
    QString status() const { return m_status; }
    QString tickSize() const { return m_tickSize; }
    QString qtyStep() const { return m_qtyStep; }
    QString minOrderQty() const { return m_minOrderQty; }
    QString maxOrderQty() const { return m_maxOrderQty; }
    QString maxMktQty() const { return m_maxMktQty; }
    QString minNotional() const { return m_minNotional; }
    QString maxLeverage() const { return m_maxLeverage; }

    bool isSame(const InstrumentInfo& other) const
    {
        return  this->m_category == other.m_category &&
               this->m_symbol == other.m_symbol &&
               this->m_baseCoin == other.m_baseCoin &&
               this->m_quoteCoin == other.m_quoteCoin &&
               this->m_status == other.m_status;
    }

    static QString makeId(EnumHelpers::MarketType market, const QString& symbol)
    {
        return EnumHelpers::marketTypeToApiString(market) + QLatin1Char(':') + symbol;
    }

    QString id() const
    {
        return makeId(m_category, m_symbol);
    }

    // Поля данных
    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown;
    QString m_symbol{};
    QString m_baseCoin{};
    QString m_quoteCoin{};
    QString m_status{};
    QString m_tickSize{};
    QString m_qtyStep{};
    QString m_minOrderQty{};
    QString m_maxOrderQty{};
    QString m_maxMktQty{};
    QString m_minNotional{};
    QString m_maxLeverage{};
};

using Instruments = QList<InstrumentInfo>;

}

#endif // INSTRUMENTINFO_HPP
