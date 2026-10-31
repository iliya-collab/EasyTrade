#ifndef DATABASEPATHPROVIDER_HPP
#define DATABASEPATHPROVIDER_HPP
#include <QString>
#include <expected>

namespace Core::Tools
{

enum class dbScope
{
    Market,
    Account
};

struct dbDescriptor
{
    dbScope scope{dbScope::Market};
    bool    testnet{false};
    QString userId;
};

class DatabasePathProvider
{
public:

    explicit DatabasePathProvider(QString baseDir);

    std::expected<QString, QString> pathFor(const dbDescriptor& d) const;
    std::expected<void, QString> validate(const dbDescriptor& d) const;

private:

    QString m_baseDir;

};

}

#endif // DATABASEPATHPROVIDER_HPP
