#pragma once
#include "BaseRepository.hpp"
#include "Tools/StdTypes.hpp"
#include "Tools/Database/IDatabaseManager.hpp"

namespace Core::Tools {

    class KlinesRepository : public BaseKlinesRepository
    {
    private:

        void handleSelectedKlines(QSqlQuery& query);

    public:

        KlinesRepository(const QString& dbPath, IDatabaseManager& manager);
        ~KlinesRepository() = default;

        bool init() override;
        bool clear() override;

        bool insert(const QList<Kline>& newKlines);
        bool insert(const Kline& newKline);

        bool select(Tools::EnumHelpers::MarketType type,
                          const QString& symbol,
                          Tools::EnumHelpers::Interval interval);

        bool select(Tools::EnumHelpers::MarketType type,
                          const QString& symbol,
                          Tools::EnumHelpers::Interval interval,
                          qint64 start, qint64 end);

    };

}