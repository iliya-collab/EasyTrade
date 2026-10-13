#pragma once
#include "PrivateServiceSignals.hpp"
#include "Tools/QueryStructures.hpp"

namespace Core::Markets
{

class IPrivateService : public QObject
{
    Q_OBJECT
public:

    IPrivateService(QObject* parent = nullptr) : QObject(parent) {}

    // Доступ к своему сигнальщику
    virtual PrivateServiceSignals* signaller() = 0;

    virtual void init(const Tools::Api& api, bool isTestnet) = 0;

    // Запрос информации об аккаунте
    virtual void requestAccountBalance() = 0;
    // Запрос информации об Api
    virtual void requestInfoAboutApi() = 0;

    // ===================================================================================
    //  Управление ордерами
    // ===================================================================================

    // POST /v5/order/create — выставить новый ордер
    virtual void requestCreateOrder(const Tools::OrderRequest& request) = 0;
    // POST /v5/order/amend — изменить параметры еще не исполненного/частично исполненного ордера
    virtual void requestAmendOrder(const Tools::OrderAmendRequest& request) = 0;
    // POST /v5/order/cancel — отменить один конкретный ордер
    virtual void requestCancelOrder(const Tools::OrderCancelRequest& request) = 0;
    // POST /v5/order/cancel-all — отменить все ордера по фильтру
    virtual void requestCancelAllOrders(const Tools::OrderCancelAllRequest& request) = 0;

    // GET /v5/order/realtime — получить список активных ордеров
    virtual void requestOpenOrders(const Tools::OpenOrdersRequest& request) = 0;
    // GET /v5/order/history — получить историю закрытых/отмененных/исполненных ордеров
    virtual void requestOrderHistory(const Tools::OrderHistoryRequest& request) = 0;
    // GET /v5/execution/list — получить историю сделок
    virtual void requestTradeHistory(const Tools::TradeHistoryRequest& request) = 0;

};

}
