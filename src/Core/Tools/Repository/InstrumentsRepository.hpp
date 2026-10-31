#ifndef INSTRUMENTSREPOSITORY_HPP
#define INSTRUMENTSREPOSITORY_HPP
#include "BaseRepository.hpp"

namespace Core::Tools
{

class InstrumentsRepository : public BaseInstrumentsRepository
{
private:

    void handleSelectedInstruments(QSqlQuery& query);

public:

    InstrumentsRepository(const QString& dbPath, IDatabaseManager& manager);
    ~InstrumentsRepository() = default;

    bool init() override;
    bool clear() override;

    bool insertInstruments(const QList<RowType>& newInstruments);
    bool selectInstruments(EnumHelpers::MarketType type);

};

}

#endif // INSTRUMENTSREPOSITORY_HPP
