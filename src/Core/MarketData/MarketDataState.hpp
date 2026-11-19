#pragma once
#include "Tools/DataModels/InstrumentModel.hpp"
#include "InstrumentData.hpp"
#include <QObject>

namespace Core {

    class MarketDataState : public QObject
    {
        Q_OBJECT

        // Readonly свойства
        Q_PROPERTY(InstrumentData*  current         READ current            NOTIFY currentChanged           FINAL)
        Q_PROPERTY(InstrumentModel* availableInsts  READ getAvailableInsts  NOTIFY availableInstsChanged    FINAL)
        Q_PROPERTY(InstrumentModel* openedInsts     READ getOpenedInsts     NOTIFY openedInstsChanged       FINAL)
        Q_PROPERTY(qint64           pingMs          READ getPingMs          NOTIFY pingMsChanged            FINAL)

    private:

        InstrumentData* find(Eh::MarketType m, const QString& symbol) const;
        InstrumentData* ensure(const InstrumentInfo& info);
        void remove(const QString& key);

        // Данные инструмета(ов)
        QHash<QString, InstrumentData*> m_data;
        InstrumentData* m_current = nullptr;
        InstrumentData* m_empty = nullptr;
        // Прочее
        std::shared_ptr<Tools::InstrumentModel> m_availableInsts{};
        std::shared_ptr<Tools::InstrumentModel> m_openedInsts{};
        QSet<QString> m_openedKeys{};
        qint64 m_pingMs = 0;

    public:

        explicit MarketDataState(QObject *parent = nullptr);

        InstrumentData* current() const;

        void openInstrument(const InstrumentInfo& info);
        void closeInstrument(const InstrumentInfo& info);
        void setCurrent(const InstrumentInfo& info);
        void resetCurrent();
        Instruments collectOpened() const;
        Q_INVOKABLE bool isOpened(const InstrumentInfo& info) const;
        Q_INVOKABLE InstrumentData* instrument(const InstrumentInfo& info) const;

        // Методы обновления
        void updateTicker(const TickerInfo&);
        void snapshotOrderbook(const OrderbookInfo&);
        void deltaUpdateOrderbook(const OrderbookInfo&);
        void updateKline(const KlineInfo&);
        void addHistoricalKlines(const Klines&);
        void updateTrades(const PublicTrades&);

        void uploadInstruments(const Instruments& instruments);
        void updatePingMs(const QString& id, qint64 pingMs);

        // READ-методы
        InstrumentModel* getAvailableInsts() const;
        InstrumentModel* getOpenedInsts() const;
        qint64 getPingMs() const;

    signals:

        void currentChanged();
        void availableInstsChanged();
        void openedInstsChanged();
        void pingMsChanged();

    };

}
