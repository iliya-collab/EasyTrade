#pragma once
#include "KlineStore.hpp"
#include <QAbstractListModel>
#include <qqmlregistration.h>

namespace Core::Tools {

class KlineSeriesModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(Core::Tools::KlineStore* klineStore READ getKlineStore WRITE setKlineStore NOTIFY klineStoreChanged FINAL)
    Q_PROPERTY(QString timeframe READ getTimeframe WRITE setTimeframe NOTIFY timeframeChanged FINAL)
    Q_PROPERTY(qint64 length READ getLength NOTIFY lengthChanged FINAL)

public:

    enum Roles {
        OpenRole = Qt::UserRole + 1,
        CloseRole,
        HighRole,
        LowRole,
        VolumeRole,
        TurnoverRole,
        TimeRole,
        ConfirmRole,
    };

    explicit KlineSeriesModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // READ методы
    Core::Tools::KlineStore* getKlineStore() const { return m_source; }
    QString getTimeframe() const { return m_timeframe; }
    qint64 getLength() const { return rowCount(); }

    // WRITE методы
    void setKlineStore(Core::Tools::KlineStore* source);
    void setTimeframe(const QString& timeframe);

    Q_INVOKABLE QVariantMap get(int index) const;
    Q_INVOKABLE QVariantMap last() const;
    Q_INVOKABLE QVariantMap first() const;

signals:

    void klineStoreChanged();
    void timeframeChanged();
    void lengthChanged();
    void modelUpdated();

private slots:

    void onSeriesReset(const QString& timeframe);
    void onKlineAppended(const QString& timeframe);
    void onKlineUpdated(const QString& timeframe);
    void onAllCleared();

private:

    void refreshAll();
    void rebuildRows();

    static QVariantMap toMap(const Kline& k);

    Core::Tools::KlineStore* m_source = nullptr;
    QString m_timeframe;
    QList<Kline> m_rows;

};

}