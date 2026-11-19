#ifndef ORDERSREPOSITORY_HPP
#define ORDERSREPOSITORY_HPP
#include "BaseRepository.hpp"
using namespace Core::Tools::Types;

namespace Core::Tools
{

class OrdersRepository : public BaseOrdersRepository
{
private:

    void handleSelectedOrders(QSqlQuery& query);

public:

    OrdersRepository(const QString& dbPath, IDatabaseManager& manager);
    ~OrdersRepository() = default;

    bool init() override;
    bool clear() override;

    bool insert(const Orders& newOrders);
    bool insert(const OrderInfo& newOrder);
    bool select(EnumHelpers::MarketType type, const QString& symbol);
};

};

#endif // ORDERSREPOSITORY_HPP
