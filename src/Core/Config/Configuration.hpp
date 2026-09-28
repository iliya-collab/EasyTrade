#pragma once
#include "Tools/StdTypes.hpp"

namespace Core
{

// Параметры конфигурации
struct ConfigurationParams
{
    // Общее
    bool m_autoConnection = true;
    QString m_activeApi = "";
    bool m_testnet = true;
    // Набор ключей
    QMap<QString, Tools::Api> m_apis{};
};

}

