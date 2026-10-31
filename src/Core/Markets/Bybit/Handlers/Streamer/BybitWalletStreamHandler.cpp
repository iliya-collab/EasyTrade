#include "BybitWalletStreamHandler.hpp"

void Core::Handlers::BybitWalletStreamHandler::handle(const QString& id, const QJsonObject &data, Streamer::PrivateStreamerSignals *signaller)
{
    if (!data.contains("data") || !data["data"].isArray())
    {
        emit signaller->errorOccurred(id, "[" + topic() + "] Invalid response structure!");
        return;
    }

    QJsonArray dataArray = data["data"].toArray();
    if (dataArray.isEmpty())
        return;

    QJsonObject dataObj = dataArray.at(0).toObject();

    Tools::AccountBalance balance{};

    balance.m_totalWalletBalance = dataObj["totalWalletBalance"].toString().toDouble();

    QJsonArray coins = dataObj["coin"].toArray();

    balance.m_assets.reserve(coins.size());

    for (const auto& coin : std::as_const(coins))
    {
        QJsonObject objCoin = coin.toObject();

        QString nameAsset = objCoin["coin"].toString();
        QString sizeAsset = objCoin["walletBalance"].toString();
        QString priceAsset = objCoin["usdValue"].toString();

        balance.m_assets.emplace_back(nameAsset, sizeAsset, priceAsset);
    }

    emit signaller->accountBalanceUpdated(balance);
}
