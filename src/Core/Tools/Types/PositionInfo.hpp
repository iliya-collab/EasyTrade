#ifndef POSITIONINFO_HPP
#define POSITIONINFO_HPP

#include "EnumHelpers.hpp"

namespace Core::Tools::Types
{

// ==========================================================================================
//  Информация о позиции (только для деривативов)
// ==========================================================================================

struct PositionInfo
{
    Q_GADGET
    QML_VALUE_TYPE(tPositionInfo)

    Q_PROPERTY(EnumHelpers::MarketType                  category            MEMBER m_category           FINAL)
    Q_PROPERTY(QString                                  symbol              MEMBER m_symbol             FINAL)
    Q_PROPERTY(EnumHelpers::OrderSide                   side                MEMBER m_side               FINAL)
    Q_PROPERTY(QString                                  size                MEMBER m_size               FINAL)
    Q_PROPERTY(int                                      positionIdx         MEMBER m_positionIdx        FINAL)
    Q_PROPERTY(QString                                  avgPrice            MEMBER m_avgPrice           FINAL)
    Q_PROPERTY(QString                                  liqPrice            MEMBER m_liqPrice           FINAL)
    Q_PROPERTY(QString                                  markPrice           MEMBER m_markPrice          FINAL)
    Q_PROPERTY(QString                                  leverage            MEMBER m_leverage           FINAL)
    Q_PROPERTY(QString                                  positionIM          MEMBER m_positionIM         FINAL)
    Q_PROPERTY(QString                                  positionMM          MEMBER m_positionMM         FINAL)
    Q_PROPERTY(QString                                  unrealisedPnl       MEMBER m_unrealisedPnl      FINAL)
    Q_PROPERTY(QString                                  cumRealisedPnl      MEMBER m_cumRealisedPnl     FINAL)
    Q_PROPERTY(int                                      adlRankIndicator    MEMBER m_adlRankIndicator   FINAL)

public:

    EnumHelpers::MarketType m_category = EnumHelpers::MarketType::Unknown; // Категория рынка (linear, inverse)
    QString m_symbol; // Торговая пара
    EnumHelpers::OrderSide m_side = EnumHelpers::OrderSide::Unknown; // Buy=Long, Sell=Short, Unknown=нет позиции
    QString m_size; // Текущий размер открытой позиции (0 если закрыта)
    int m_positionIdx = 0; // Режим: 0-OneWay, 1-Hedge Long, 2-Hedge Short

    // Финансовые метрики риска
    QString m_avgPrice; // Средняя цена входа
    QString m_liqPrice; // Цена ликвидации
    QString m_markPrice; // Текущая цена маркировки
    QString m_leverage; // Плечо
    QString m_positionIM; // Начальная маржа под позицию
    QString m_positionMM; // Поддерживающая маржа

    // Результат
    QString m_unrealisedPnl; // Нереализованный PnL
    QString m_cumRealisedPnl; // Накопленный реализованный PnL

    int m_adlRankIndicator = 0; // Очередь на авто-делеверидж (0-5)
};

}

#endif // POSITIONINFO_HPP
