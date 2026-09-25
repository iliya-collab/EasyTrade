#pragma once
#include <Tools/StdTypes.hpp>
#include <QObject>
#include <QMap>
#include <QtQml>

namespace Core::Tools {

class KlineStore : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:

    explicit KlineStore(QObject* parent = nullptr) : QObject(parent) {}

    // Для потоковых данных
    void updateKline(const Kline& kline);
    void addKline(const Kline& kline);

    // Для данных из сети
    void addHistoricalKlines(const QList<Kline>& klines);

    // Доступ к серии конкретного таймфрейма.
    Q_INVOKABLE QList<Kline> series(const QString& timeframe) const;
    const QMap<qint64, Kline>& seriesMap(const QString& timeframe) const;
    int seriesSize(const QString& timeframe) const;
    bool lastKline(const QString& timeframe, Kline& out) const;

    Q_INVOKABLE void clear();
    Q_INVOKABLE void clearTimeframe(const QString& timeframe);
    Q_INVOKABLE bool hasTimeframe(const QString& timeframe) const;

signals:

    // Структурное изменение серии (история добавлена/очищена)
    void seriesReset(const QString& timeframe);
    // Добавлена новая (открытая) свеча в конец серии
    void klineAppended(const QString& timeframe);
    // Обновлена последняя (ещё не закрытая) свеча
    void klineUpdated(const QString& timeframe);
    // Очищены вообще все серии
    void allCleared();

private:

    static const QList<Kline> s_empty;
    //QMap<QString, QList<Kline>> m_klineSeries{}; // timeframe -> серия свечей
    QMap<QString, QMap<qint64, Kline>> m_klineSeries{}; // timeframe -> (open time -> kline)

};

}