#pragma once
#include "BaseRepository.hpp"
#include "Tools/StdTypes.hpp"
#include "Tools/Database/IDatabaseManager.hpp"

namespace Core::Tools
{

class PublicTradesRepository : public BasePublicTradesRepository
{
private:

    void handleSelectedPublicTrades(QSqlQuery& query);

public:

    PublicTradesRepository(const QString& dbPath, IDatabaseManager& manager);
    ~PublicTradesRepository() = default;

    bool init() override;
    bool open() override;
    void close() override;
    QString error() override;
    bool clear() override;

    bool insertPublicTrades(const PublicTrades& newPublicTrades);
    bool selectKlines(Tools::MarketType type, const QString& symbol);

};

}
