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
        bool open() override;
        void close() override;
        QString error() override;
        bool clear() override;

        bool insertKlines(const QList<Kline>& newKlines);
        bool insertKline(const Kline& newKline);
        bool selectKlines(Tools::EnumHelpers::MarketType type, const QString& symbol, Tools::EnumHelpers::Interval interval);
        bool selectKlines(Tools::EnumHelpers::MarketType type, const QString& symbol, Tools::EnumHelpers::Interval interval, qint64 start, qint64 end);

    };

}