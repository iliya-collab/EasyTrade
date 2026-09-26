#include "GetBybitInfoAboutAPIHandler.hpp"

namespace Core::Markets
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

    void GetBybitInfoAboutAPIHandler::handle(const QJsonObject &data, const QVariant& requestContext, IPrivateService *service)
    {
        if (!data.contains("retCode") || data["retCode"].toInt() != 0)
        {
            emit service->errorOccurred(QString("Error processing request [endpoint = %1]: " +
                                                data["retMsg"].toString()).arg(endpoint()));
            return;
        }

        //qInfo() << QJsonDocument(data).toJson(QJsonDocument::Indented);
        //qInfo().noquote() << QJsonDocument(data).toJson();

        Tools::ApiInfo info{};

        QJsonObject result = data["result"].toObject();

        info.m_note = result["note"].toString();

        QJsonArray ipsArr = result["ips"].toArray();
        bool anyIp = ipsArr.size() == 1 && ipsArr[0].toString() == "*";
        if (!anyIp)
        {
            info.m_ips.reserve(ipsArr.size());
            for (const auto& ip : std::as_const(ipsArr))
                info.m_ips.emplaceBack(ip.toString());
        }

        info.m_expiredAt = parseISOFormat(result["expiredAt"].toString());
        info.m_createdAt = parseISOFormat(result["createdAt"].toString());
        info.m_deadlineDay = result["deadlineDay"].toInt();
        info.m_vipLevel = result["vipLevel"].toString();
        info.m_isUnifiedAccount = result["uta"].toInt() == 1;
        info.m_isMaster = result["isMaster"].toBool();
        info.m_kycLevel = result["kycLevel"].toString();
        info.m_userId = QString::number(result["userID"].toInt());

        info.m_readOnly = result["readOnly"].toVariant().toBool();
        QJsonObject permissionsObj = result["permissions"].toObject();

        // Хелпер-лямбда для быстрой проверки строки внутри JSON-массива
        auto hasField = [](const QJsonArray &arr, const QString &val) -> bool {
            for (const auto& item : std::as_const(arr)) {
                if (item.toString() == val) return true;
            }
            return false;
        };

        // Контракты (Деривативы)
        if (permissionsObj.contains("ContractTrade")) {
            QJsonArray contractArr = permissionsObj["ContractTrade"].toArray();
            info.m_permissionOrderContract = hasField(contractArr, "Order");
            info.m_permissionPositionContract = hasField(contractArr, "Position");
        }

        // Спот
        if (permissionsObj.contains("Spot")) {
            info.m_permissionSpotTrade = hasField(permissionsObj["Spot"].toArray(), "SpotTrade");
        }

        // Опционы
        if (permissionsObj.contains("Options")) {
            info.m_permissionOptionsTrade = hasField(permissionsObj["Options"].toArray(), "OptionsTrade");
        }

        // Конвертация
        if (permissionsObj.contains("Exchange")) {
            info.m_permissionExchange = hasField(permissionsObj["Exchange"].toArray(), "ExchangeHistory");
        }

        // Earn продукты
        if (permissionsObj.contains("Earn")) {
            info.m_permissionEarn = hasField(permissionsObj["Earn"].toArray(), "Earn");
        }

        // Кошелек и переводы
        if (permissionsObj.contains("Wallet")) {
            QJsonArray walletArr = permissionsObj["Wallet"].toArray();
            info.m_permissionWithdraw = hasField(walletArr, "Withdraw");
            info.m_permissionAccountTransfer = hasField(walletArr, "AccountTransfer");

            // Проверяем права на суб-переводы (зависит от того мастер-ключ или суб-ключ)
            info.m_permissionSubTransfer = hasField(walletArr, "SubMemberTransfer") ||
                                           hasField(walletArr, "SubMemberTransferList");
        }

        emit service->infoAboutApiReceived(info);
    }

}