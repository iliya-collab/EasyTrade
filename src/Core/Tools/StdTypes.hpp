#ifndef STDTYPES_HPP
#define STDTYPES_HPP

#include "RegistrationTools.hpp"
#include "Types/RegistrationToolsTypes.hpp"

#include "Types/EnumHelpers.hpp"
#include "Types/InstrumentInfo.hpp"
#include "Types/TickerInfo.hpp"
#include "Types/OrderbookInfo.hpp"
#include "Types/KlineInfo.hpp"
#include "Types/PublicTradeInfo.hpp"
#include "Types/ApiInfo.hpp"
#include "Types/WalletInfo.hpp"
#include "Types/OrderInfo.hpp"
#include "Types/ExecutionInfo.hpp"
#include "Types/PositionInfo.hpp"

using namespace Core::Tools;
using namespace Core::Tools::Types;
using Eh = EnumHelpers;

// РЕГИСТРАЦИЯ В МЕТАСИСТЕМЕ QT
Q_DECLARE_METATYPE(Core::Tools::Types::OrderbookInfo)
Q_DECLARE_METATYPE(Core::Tools::Types::TickerInfo)
Q_DECLARE_METATYPE(Core::Tools::Types::KlineInfo)
Q_DECLARE_METATYPE(Core::Tools::Types::PublicTradeInfo)
Q_DECLARE_METATYPE(Core::Tools::Types::InstrumentInfo)
Q_DECLARE_METATYPE(Core::Tools::Types::Api)
Q_DECLARE_METATYPE(Core::Tools::Types::ApiInfo)
Q_DECLARE_METATYPE(Core::Tools::Types::AssetInfo)
Q_DECLARE_METATYPE(Core::Tools::Types::WalletInfo)
Q_DECLARE_METATYPE(Core::Tools::Types::OrderInfo)
Q_DECLARE_METATYPE(Core::Tools::Types::ExecutionInfo)
Q_DECLARE_METATYPE(Core::Tools::Types::PositionInfo)

#endif