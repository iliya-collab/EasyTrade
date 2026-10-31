#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(lcCore)
Q_DECLARE_LOGGING_CATEGORY(lcMarket)
Q_DECLARE_LOGGING_CATEGORY(lcAccount)

#define coreDebug()    qCDebug(lcCore)
#define coreInfo()     qCInfo(lcCore)
#define coreWarn()     qCWarning(lcCore)
#define coreCrit()     qCCritical(lcCore)

#define marketDebug()  qCDebug(lcMarket)
#define marketInfo()   qCInfo(lcMarket)
#define marketWarn()   qCWarning(lcMarket)
#define marketCrit()   qCCritical(lcMarket)

#define accountDebug() qCDebug(lcAccount)
#define accountInfo()  qCInfo(lcAccount)
#define accountWarn()  qCWarning(lcAccount)
#define accountCrit()  qCCritical(lcAccount)

#endif // LOGGING_HPP
