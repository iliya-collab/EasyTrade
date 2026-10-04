#pragma once
#include "Tools/StdTypes.hpp"

namespace Core
{

// Параметры конфигурации
struct ConfigurationParams
{
    // Общее
    bool m_autoConnection = true;
    bool m_enableTrades = false;
    bool m_testnet = true;
    QString m_activeApi = "";
    // Набор ключей
    QMap<QString, Tools::Api> m_apis{};
};

}

