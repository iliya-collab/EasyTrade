#pragma once
#include "Tools//Database/IDatabaseManager.hpp"
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

        virtual bool init() = 0;
        virtual bool open() = 0;
        virtual void close() = 0;
        virtual QString error() = 0;
        virtual bool clear() = 0;

        QList<TRow> getData() const { return m_data; };

        BaseRepository(const QString& dbPath, IDatabaseManager& manager) :
            m_dbPath(dbPath), m_dbManager(manager) {};
        virtual ~BaseRepository() {}

    };

    using BaseCryptoRepository = BaseRepository<Tools::TradeInfo>;
    using BaseKlinesRepository = BaseRepository<Tools::Kline>;
    using BasePublicTradesRepository = BaseRepository<Tools::PublicTradeItem>;

}