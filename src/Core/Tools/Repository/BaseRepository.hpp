#pragma once
#include "Tools/Database/IDatabaseManager.hpp"
#include "Tools/StdTypes.hpp"
#include <QString>

namespace Core::Tools {

    template <typename TRow>
    class BaseRepository
    {
    protected:

        QString m_dbPath = "";
        QList<TRow> m_data{};
        IDatabaseManager& m_dbManager;

    public:

        using RowType = TRow;

        BaseRepository(const QString& dbPath, IDatabaseManager& manager) : m_dbPath(dbPath), m_dbManager(manager) {};
        virtual ~BaseRepository() = default;

        virtual bool init() = 0;

        virtual bool open() {
            return m_dbManager.open(m_dbPath);
        }

        virtual void close() {
            m_dbManager.close(m_dbPath);
        }

        virtual QString error() {
            return m_dbManager.error();
        }

        virtual bool clear() = 0;

        QList<TRow> getData() const { return m_data; };


    };

    using BaseInstrumentsRepository = BaseRepository<Tools::InstrumentInfo>;
    using BaseKlinesRepository = BaseRepository<Tools::Kline>;
    using BasePublicTradesRepository = BaseRepository<Tools::PublicTradeItem>;
    using BaseOrdersRepository = BaseRepository<Tools::OrderInfo>;
    using BaseExecutionsRepository = BaseRepository<Tools::ExecutionInfo>;

}