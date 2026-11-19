#include "GetBybitInfoAboutAPIHandler.hpp"

namespace Core::Handlers
{

QString GetBybitInfoAboutAPIHandler::parseISOFormat(const QString &time)
{
    QDateTime utcTime = QDateTime::fromString(time, Qt::ISODate);

    if (!utcTime.isValid())
    {
        qWarning() << "Could not recognize the date:" << time;
        return time;
    }

    if (utcTime.toMSecsSinceEpoch() == 0)
    {
        qDebug() << "The key is perpetual (tied to an IP address)";
        return "";
    }

    QDateTime localTime = utcTime.toLocalTime();

    return localTime.toString("yyyy-MM-dd");
}

void GetBybitInfoAboutAPIHandler::handle(const QJsonObject &data, const QVariant& requestContext, Service::PrivateServiceSignals *signaller)
{
    if (!data.contains("retCode") || data["retCode"].toInt() != 0)
    {
        emit signaller->errorOccurred(QString("Error processing request [endpoint = %1]: " + data["retMsg"].toString()).arg(endpoint()));
        return;
    }

    ApiInfo info{};

    QJsonObject result = data["result"].toObject();

    info.note = result["note"].toString();

    QJsonArray ipsArr = result["ips"].toArray();
    bool anyIp = ipsArr.size() == 1 && ipsArr[0].toString() == "*";
    if (!anyIp)
    {
        info.ips.reserve(ipsArr.size());
        for (const auto& ip : std::as_const(ipsArr))
            info.ips.emplaceBack(ip.toString());
    }

    info.expiredAt = parseISOFormat(result["expiredAt"].toString());
    info.createdAt = parseISOFormat(result["createdAt"].toString());
    info.deadlineDay = result["deadlineDay"].toInt();
    info.vipLevel = result["vipLevel"].toString();
    info.isUnifiedAccount = result["uta"].toInt() == 1;
    info.isMaster = result["isMaster"].toBool();
    info.kycLevel = result["kycLevel"].toString();
    info.userId = QString::number(result["userID"].toInt());

    info.readOnly = result["readOnly"].toVariant().toBool();
    QJsonObject permissionsObj = result["permissions"].toObject();

    auto hasField = [](const QJsonArray &arr, const QString &val) -> bool {
        for (const auto& item : std::as_const(arr)) {
            if (item.toString() == val) return true;
        }
        return false;
    };

    // Контракты (Деривативы)
    if (permissionsObj.contains("ContractTrade")) {
        QJsonArray contractArr = permissionsObj["ContractTrade"].toArray();
        info.permissionOrderContract = hasField(contractArr, "Order");
        info.permissionPositionContract = hasField(contractArr, "Position");
    }

    // Спот
    if (permissionsObj.contains("Spot")) {
        info.permissionSpotTrade = hasField(permissionsObj["Spot"].toArray(), "SpotTrade");
    }

    // Опционы
    if (permissionsObj.contains("Options")) {
        info.permissionOptionsTrade = hasField(permissionsObj["Options"].toArray(), "OptionsTrade");
    }

    // Конвертация
    if (permissionsObj.contains("Exchange")) {
        info.permissionExchange = hasField(permissionsObj["Exchange"].toArray(), "ExchangeHistory");
    }

    // Earn продукты
    if (permissionsObj.contains("Earn")) {
        info.permissionEarn = hasField(permissionsObj["Earn"].toArray(), "Earn");
    }

    // Кошелек и переводы
    if (permissionsObj.contains("Wallet")) {
        QJsonArray walletArr = permissionsObj["Wallet"].toArray();
        info.permissionWithdraw = hasField(walletArr, "Withdraw");
        info.permissionAccountTransfer = hasField(walletArr, "AccountTransfer");

        // Проверяем права на суб-переводы (зависит от того мастер-ключ или суб-ключ)
        info.permissionSubTransfer = hasField(walletArr, "SubMemberTransfer") ||
                                       hasField(walletArr, "SubMemberTransferList");
    }

    emit signaller->infoAboutApiReceived(info);
}

}