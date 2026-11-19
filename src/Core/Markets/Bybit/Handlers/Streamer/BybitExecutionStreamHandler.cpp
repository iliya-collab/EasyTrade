#include "BybitExecutionStreamHandler.hpp"

void Core::Handlers::BybitExecutionStreamHandler::handle(const QString& id, const QJsonObject &data, Streamer::PrivateStreamerSignals *signaller)
{
    if (!data.contains("data") || !data["data"].isArray())
    {
        emit signaller->errorOccurred(id, "[" + topic() + "] Invalid response structure!");
        return;
    }

    QJsonArray dataArray = data["data"].toArray();
    if (dataArray.isEmpty())
        return;

    for (const auto& iData : std::as_const(dataArray))
    {
        QJsonObject dataObj = iData.toObject();

        ExecutionInfo exec{};

        exec.m_category    = EnumHelpers::apiStringToMarketType(dataObj["category"].toString());
        exec.m_symbol      = dataObj["symbol"].toString();
        exec.m_execId      = dataObj["execId"].toString();
        exec.m_orderId     = dataObj["orderId"].toString();
        exec.m_orderLinkId = dataObj["orderLinkId"].toString();
        exec.m_feeCurrency = dataObj["feeCurrency"].toString();

        QString strSide = dataObj["side"].toString();
        if (strSide == "Buy")
            exec.m_side = EnumHelpers::OrderSide::Buy;
        else if (strSide == "Sell")
            exec.m_side = EnumHelpers::OrderSide::Sell;
        else
            exec.m_side = EnumHelpers::OrderSide::Unknown;

        QString strExecType = dataObj["execType"].toString();
        if (strExecType == "Trade")
            exec.m_execType = EnumHelpers::ExecType::Trade;
        else if (strExecType == "Adl")
            exec.m_execType = EnumHelpers::ExecType::Adl;
        else if (strExecType == "Funding")
            exec.m_execType = EnumHelpers::ExecType::Funding;
        else if (strExecType == "Liquidation" || strExecType == "BustTrade")
            exec.m_execType = EnumHelpers::ExecType::Liquidation;
        else
            exec.m_execType = EnumHelpers::ExecType::Unknown;

        exec.m_execPrice   = dataObj["execPrice"].toString();
        exec.m_execQty     = dataObj["execQty"].toString();
        exec.m_execValue   = dataObj["execValue"].toString();
        exec.m_execFee     = dataObj["execFee"].toString();

        exec.m_isMaker     = dataObj["isMaker"].toBool();
        exec.m_execTime    = dataObj["execTime"].toString().toLongLong();

        emit signaller->executionUpdated(exec);
    }
}
