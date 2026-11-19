#pragma once
#include "Tools/StdTypes.hpp"

namespace Core
{

// Параметры конфигурации
struct ConfigurationParams
{
    // Общее
    bool autoConnection = true;
    bool enableTrades = false;
    bool testnet = true;
    QString activeApi = "";
    // Набор ключей
    QMap<QString, Api> apis{};
};

}

