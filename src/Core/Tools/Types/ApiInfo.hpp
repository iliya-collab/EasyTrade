#ifndef APIINFO_HPP
#define APIINFO_HPP
#include "RegistrationToolsTypes.hpp"
#include <QString>
#include <QList>

namespace Core::Tools::Types
{
// ==========================================================================================
//  Информация об API-ключ
// ==========================================================================================

struct ApiInfo
{
    /*Q_GADGET
    QML_VALUE_TYPE(tApiInfo)

    Q_PROPERTY(bool readOnly MEMBER m_readOnly FINAL)
    Q_PROPERTY(bool permissionSpotTrade MEMBER m_permissionSpotTrade FINAL)
    Q_PROPERTY(bool permissionOrderContract MEMBER m_permissionOrderContract FINAL)
    Q_PROPERTY(bool permissionPositionContract MEMBER m_permissionPositionContract FINAL)
    Q_PROPERTY(bool permissionOptionsTrade MEMBER m_permissionOptionsTrade FINAL)
    Q_PROPERTY(bool permissionWithdraw MEMBER m_permissionWithdraw FINAL)
    Q_PROPERTY(bool permissionExchange MEMBER m_permissionExchange FINAL)
    Q_PROPERTY(bool permissionEarn MEMBER m_permissionEarn FINAL)
    Q_PROPERTY(bool permissionSubTransfer MEMBER m_permissionSubTransfer FINAL)
    Q_PROPERTY(bool permissionAccountTransfer MEMBER m_permissionAccountTransfer FINAL)

    Q_PROPERTY(QString note MEMBER m_note FINAL)
    Q_PROPERTY(QStringList ips MEMBER m_ips FINAL)
    Q_PROPERTY(QString expiredAt MEMBER m_expiredAt FINAL)
    Q_PROPERTY(QString createdAt MEMBER m_createdAt FINAL)
    Q_PROPERTY(int deadlineDay MEMBER m_deadlineDay FINAL)
    Q_PROPERTY(QString vipLevel MEMBER m_vipLevel FINAL)
    Q_PROPERTY(bool isUnifiedAccount MEMBER m_isUnifiedAccount FINAL)
    Q_PROPERTY(bool isMaster MEMBER m_isMaster FINAL)
    Q_PROPERTY(QString kycLevel MEMBER m_kycLevel FINAL)
    Q_PROPERTY(QString userId MEMBER m_userId FINAL)

    Q_PROPERTY(bool isPublic READ isPublic FINAL)
    Q_PROPERTY(bool isDangerous READ isDangerous FINAL)
    Q_PROPERTY(bool canTrade READ canTrade FINAL)

public:*/

    // Базовые поля
    QString note{};                           // Имя ключа
    QList<QString> ips{};                     // IP адреса, которые могут исп этот ключ
    QString expiredAt{};                      // Дата истечения
    QString createdAt{};                      // Дата создания
    int deadlineDay{};                        // Дней до истечения
    QString vipLevel{"Regular"};              // Уровень VIP
    bool isUnifiedAccount = false;            // true = UTA аккаунт, false = обычный
    bool isMaster = true;                     // Главный аккаунт или суб-аккаунт
    QString kycLevel{"LEVEL_DEFAULT"};        // Уровень верификации
    QString userId{};                         // UID аккаунта

    // Права
    bool readOnly = true;                     // Только для чтения
    bool permissionSpotTrade = false;         // Разрешение к спотовой торговли
    bool permissionOrderContract = false;     // Право на выставление ордеров
    bool permissionPositionContract = false;  // Право на управление позициями
    bool permissionWithdraw = false;          // Разрешение к выводу активов с биржи
    bool permissionAccountTransfer = false;   // Разрешение к переводу между внутренними кошельками
    bool permissionOptionsTrade = false;      // Опционы
    bool permissionExchange = false;           // Быстрая конвертация (ExchangeHistory)
    bool permissionEarn = false;              // Bybit Earn / Лендинг (Earn)
    bool permissionSubTransfer = false;       // Переводы между суб-аккаунтами


    // Хелпер: проверка привязки к IP
    bool isPublic() const noexcept {
        return ips.isEmpty() || ips.contains("*");
    }

    // Хелпер безопасности: ключ без IP-привязки, но с правами на вывод или торговлю
    bool isDangerous() const noexcept {
        bool hasIpBinding = !ips.isEmpty() && !ips.contains("*");
        return (permissionSpotTrade || permissionOrderContract || permissionWithdraw) && !hasIpBinding;
    }

    // Хелпер: Можно ли отправлять торговые ордера
    bool canTrade() const noexcept {
        if (readOnly)
            return false; // Ключ только для чтения
        return permissionSpotTrade || permissionOrderContract || permissionOptionsTrade;
    }

    bool operator==(const ApiInfo &other) const noexcept {
        return readOnly                       == other.readOnly
               && permissionSpotTrade         == other.permissionSpotTrade
               && permissionOrderContract     == other.permissionOrderContract
               && permissionPositionContract  == other.permissionPositionContract
               && permissionWithdraw          == other.permissionWithdraw
               && permissionAccountTransfer   == other.permissionAccountTransfer
               && permissionOptionsTrade      == other.permissionOptionsTrade
               && permissionExchange          == other.permissionExchange
               && permissionEarn              == other.permissionEarn
               && permissionSubTransfer       == other.permissionSubTransfer
               && isUnifiedAccount            == other.isUnifiedAccount
               && isMaster                    == other.isMaster
               && deadlineDay                 == other.deadlineDay
               && expiredAt                   == other.expiredAt
               && createdAt                   == other.createdAt
               && note                        == other.note
               && vipLevel                    == other.vipLevel
               && kycLevel                    == other.kycLevel
               && userId                      == other.userId
               && ips                         == other.ips;
    }


    bool operator!=(const ApiInfo &other) const noexcept {
        return !(*this == other);
    }

};

// ==========================================================================================
//  API-ключ
// ==========================================================================================

struct Api
{
    /*Q_GADGET
    QML_VALUE_TYPE(tApi)

    Q_PROPERTY(QString apiKey MEMBER m_apiKey FINAL)
    Q_PROPERTY(QString secretKey MEMBER m_secretKey FINAL)
    Q_PROPERTY(ApiInfo info MEMBER m_info FINAL)

public:*/

    QString apiKey = "";                  // Api ключ
    QString secretKey = "";               // Секретный Api ключ
    /*ApiInfo m_info{};

    bool operator==(const Api &other) const noexcept {
        return m_apiKey       == other.m_apiKey
               && m_secretKey == other.m_secretKey
               && m_info      == other.m_info;
    }

    bool operator!=(const Api &other) const noexcept {
        return !(*this == other);
    }*/

};
}

#endif // APIINFO_HPP
