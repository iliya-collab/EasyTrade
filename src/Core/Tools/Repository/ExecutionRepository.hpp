#ifndef EXECUTIONREPOSITORY_HPP
#define EXECUTIONREPOSITORY_HPP
#include "BaseRepository.hpp"

namespace Core::Tools
{

class ExecutionRepository : public BaseExecutionsRepository
{
private:

    void handleQuery(QSqlQuery& query);

public:

    ExecutionRepository(const QString& dbPath, IDatabaseManager& manager);
    ~ExecutionRepository() = default;

    bool init() override;
    bool clear() override;

    bool insert(const Executions& newExecs);
    bool insert(const ExecutionInfo& newExec);
    bool select(Tools::EnumHelpers::MarketType type, const QString& symbol);
};

};

#endif // EXECUTIONREPOSITORY_HPP
