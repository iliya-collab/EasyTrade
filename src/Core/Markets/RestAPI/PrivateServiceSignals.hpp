#ifndef PRIVATESERVICESIGNALS_HPP
#define PRIVATESERVICESIGNALS_HPP
#include <QObject>
#include "Tools/StdTypes.hpp"

namespace Core::Markets
{

class PrivateServiceSignals : public QObject
{
    Q_OBJECT

public:

    explicit PrivateServiceSignals(QObject* parent = nullptr) : QObject(parent) {}

signals:

    void accountBalanceReceived(const Core::Tools::AccountBalance& balance);
    void accountVerified();
    void infoAboutApiReceived(const Core::Tools::ApiInfo& apiInfo);

    // ===================================================================================
    //  Ответы на управление ордерами
    // ===================================================================================

    // Ордер успешно создан
    void orderCreated(const QString& orderId, const QString& orderLinkId);
    // Ордер успешно изменен
    void orderAmended(const QString& orderId, const QString& orderLinkId);
    // Ордер успешно отменен
    void orderCancelled(const QString& orderId, const QString& orderLinkId);
    // Массовая отмена выполнена — список ID отмененных ордеров
    void allOrdersCancelled(const QList<QString>& cancelledOrderIds);
    // Получен список активных ордеров
    void openOrdersReceived(const QList<Core::Tools::OrderInfo>& orders, const QString& nextPageCursor);
    void allOpenOrdersReceived(const QList<Core::Tools::OrderInfo>& orders);
    // Получена история ордеров
    void orderHistoryReceived(const QList<Core::Tools::OrderInfo>& orders, const QString& nextPageCursor);
    void allOrderHistoryReceived(const QList<Core::Tools::OrderInfo>& orders);
    // Получена истории сделак
    void tradeHistoryReceived(const QList<Core::Tools::ExecutionInfo>& execs, const QString& nextPageCursor);
    void allTradeHistoryLoaded(const QList<Core::Tools::ExecutionInfo>& execs);

    // ===================================================================================
    //  Ответы на ошибки
    // ===================================================================================

    // Отдельный канал для отказов конкретно по управлению ордерами
    void orderRequestRejected(const QString& orderLinkId, const QString& reason);
    // Общий канал ошибок
    void errorOccurred(const QString& error);

    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
};

}

#endif // PRIVATESERVICESIGNALS_HPP
