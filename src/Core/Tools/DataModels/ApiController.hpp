#ifndef APICONTROLLER_HPP
#define APICONTROLLER_HPP
#include <QObject>
#include <QStringList>
#include "Tools/StdTypes.hpp"

namespace Core::Tools
{

class ApiController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("ApiController connot be created in QML")

    Q_PROPERTY(QString     apiKey                     READ apiKey                     NOTIFY apiChanged  FINAL)

    Q_PROPERTY(QString     note                       READ note                       NOTIFY infoChanged FINAL)
    Q_PROPERTY(QStringList ips                        READ ips                        NOTIFY infoChanged FINAL)
    Q_PROPERTY(QString     expiredAt                  READ expiredAt                  NOTIFY infoChanged FINAL)
    Q_PROPERTY(QString     createdAt                  READ createdAt                  NOTIFY infoChanged FINAL)
    Q_PROPERTY(int         deadlineDay                READ deadlineDay                NOTIFY infoChanged FINAL)
    Q_PROPERTY(QString     vipLevel                   READ vipLevel                   NOTIFY infoChanged FINAL)
    Q_PROPERTY(QString     kycLevel                   READ kycLevel                   NOTIFY infoChanged FINAL)
    Q_PROPERTY(QString     userId                     READ userId                     NOTIFY infoChanged FINAL)
    Q_PROPERTY(bool        isUnifiedAccount           READ isUnifiedAccount           NOTIFY infoChanged FINAL)
    Q_PROPERTY(bool        isMaster                   READ isMaster                   NOTIFY infoChanged FINAL)

    Q_PROPERTY(bool        readOnly                   READ readOnly                   NOTIFY infoChanged FINAL)
    Q_PROPERTY(bool        permissionSpotTrade        READ permissionSpotTrade        NOTIFY infoChanged FINAL)
    Q_PROPERTY(bool        permissionOrderContract    READ permissionOrderContract    NOTIFY infoChanged FINAL)
    Q_PROPERTY(bool        permissionPositionContract READ permissionPositionContract NOTIFY infoChanged FINAL)
    Q_PROPERTY(bool        permissionOptionsTrade     READ permissionOptionsTrade     NOTIFY infoChanged FINAL)
    Q_PROPERTY(bool        permissionWithdraw         READ permissionWithdraw         NOTIFY infoChanged FINAL)
    Q_PROPERTY(bool        permissionExchange         READ permissionExchange         NOTIFY infoChanged FINAL)
    Q_PROPERTY(bool        permissionEarn             READ permissionEarn             NOTIFY infoChanged FINAL)
    Q_PROPERTY(bool        permissionSubTransfer      READ permissionSubTransfer      NOTIFY infoChanged FINAL)
    Q_PROPERTY(bool        permissionAccountTransfer  READ permissionAccountTransfer  NOTIFY infoChanged FINAL)

    Q_PROPERTY(bool        isPublic                   READ isPublic                   NOTIFY infoChanged FINAL)
    Q_PROPERTY(bool        isDangerous                READ isDangerous                NOTIFY infoChanged FINAL)
    Q_PROPERTY(bool        canTrade                   READ canTrade                   NOTIFY infoChanged FINAL)

public:
    explicit ApiController(QObject* parent = nullptr) : QObject(parent) {}

    // --- C++ ---
    const Api& api() const { return m_api; }
    void setApi(const Api& api);            // ключ
    void setInfo(const ApiInfo& info);      // info (ответ биржи про ключ)
    void clear();

    // --- QML ---
    QString     apiKey()                     const { return m_api.apiKey; }
    QString     note()                       const { return m_info.note; }
    QStringList ips()                        const { return m_info.ips; }
    QString     expiredAt()                  const { return m_info.expiredAt; }
    QString     createdAt()                  const { return m_info.createdAt; }
    int         deadlineDay()                const { return m_info.deadlineDay; }
    QString     vipLevel()                   const { return m_info.vipLevel; }
    QString     kycLevel()                   const { return m_info.kycLevel; }
    QString     userId()                     const { return m_info.userId; }
    bool        isUnifiedAccount()           const { return m_info.isUnifiedAccount; }
    bool        isMaster()                   const { return m_info.isMaster; }
    bool        readOnly()                   const { return m_info.readOnly; }
    bool        permissionSpotTrade()        const { return m_info.permissionSpotTrade; }
    bool        permissionOrderContract()    const { return m_info.permissionOrderContract; }
    bool        permissionPositionContract() const { return m_info.permissionPositionContract; }
    bool        permissionOptionsTrade()     const { return m_info.permissionOptionsTrade; }
    bool        permissionWithdraw()         const { return m_info.permissionWithdraw; }
    bool        permissionExchange()         const { return m_info.permissionExchange; }
    bool        permissionEarn()             const { return m_info.permissionEarn; }
    bool        permissionSubTransfer()      const { return m_info.permissionSubTransfer; }
    bool        permissionAccountTransfer()  const { return m_info.permissionAccountTransfer; }
    bool        isPublic()                   const { return m_info.isPublic(); }
    bool        isDangerous()                const { return m_info.isDangerous(); }
    bool        canTrade()                   const { return m_info.canTrade(); }

signals:
    void apiChanged();
    void infoChanged();

private:
    Api m_api{};
    ApiInfo m_info{};
};

}

#endif // APICONTROLLER_HPP