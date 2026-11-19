#ifndef PRIVATESERVICESIGNALS_HPP
#define PRIVATESERVICESIGNALS_HPP
#include "ServiceSignals.hpp"
#include "Tools/StdTypes.hpp"

namespace Core::Service
{

class PrivateServiceSignals : public ServiceSignals
{
    Q_OBJECT

public:

    explicit PrivateServiceSignals(QObject* parent = nullptr) : ServiceSignals(parent) {}

signals:

    void accountVerified();
    void walletReceived(const WalletInfo& wallet);
    void infoAboutApiReceived(const ApiInfo& apiInfo);

    // Ордер успешно создан
    void orderCreated(const QString& orderId, const QString& orderLinkId);
    // Ордер успешно изменен
    void orderAmended(const QString& orderId, const QString& orderLinkId);
    // Ордер успешно отменен
    void orderCancelled(const QString& orderId, const QString& orderLinkId);
    // Массовая отмена выполнена — список ID отмененных ордеров
    void allOrdersCancelled(const QList<QString>& cancelledOrderIds);
    // Получен список активных ордеров
    void allOpenOrdersReceived(const QList<OrderInfo>& orders, bool wasTruncated);
    // Получена история ордеров
    void allOrderHistoryReceived(const QList<OrderInfo>& orders, bool wasTruncated);
    // Получена истории сделак
    void allTradeHistoryReceived(const QList<ExecutionInfo>& execs, bool wasTruncated);

    // Получение комиссии исполнения
    void feeRatesReceived(const FeeRatesInfo& rates);

    // Сигналы с пагинацией
    void tradeHistoryPageReceived(const QList<ExecutionInfo>& execs, const QString& nextPageCursor, const QVariant& context);
    void orderHistoryPageReceived(const QList<OrderInfo>& orders, const QString& nextPageCursor, const QVariant& context);
    void openOrdersPageReceived(const QList<OrderInfo>& orders, const QString& nextPageCursor, const QVariant& context);

    // Отдельный канал для отказов конкретно по управлению ордерами
    void orderRequestRejected(const QString& orderLinkId, const QString& reason);
};

}

#endif // PRIVATESERVICESIGNALS_HPP
