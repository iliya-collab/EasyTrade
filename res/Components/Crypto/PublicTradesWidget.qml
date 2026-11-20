import QtQuick 2.15
import Application.Core 1.0
import Components.Configs 1.0

Item {
    id: root

    readonly property real priceField: TradesConfig.priceField
    readonly property real volumeField: TradesConfig.volumeField
    readonly property real turnoverField: TradesConfig.turnoverField
    readonly property real timeField: TradesConfig.timeField
    readonly property var recentTrades: ReversedProxyModel {
        sourceModel: AppCore.marketState.current.trades
    }

    ListView {
        id: tradeView
        anchors.fill: parent
        anchors.margins: TradesConfig.margins
        model: root.recentTrades
        clip: true
        reuseItems: true

        delegate: Row {
            width: tradeView.width
            padding: TradesConfig.rowPadding

            property var priceStr:    TradesConfig.formatPrice(tradePrice)
            property var sideColor:   TradesConfig.sideColor(tradeSide)
            property var volumeStr:   TradesConfig.formatAmount(tradeVolume)
            property var turnoverStr: TradesConfig.formatAmount(tradeTurnover)
            property var timeStr:     TradesConfig.formatTime(tradeTime)

            // Цена
            Text {
                text: priceStr
                width: tradeView.width * root.priceField
                color: sideColor
                font.pixelSize: TradesConfig.fontSize
                font.family: TradesConfig.fontFamily
            }

            // ОбЪем
            Text {
                text: volumeStr
                width: tradeView.width * root.volumeField
                color: TradesConfig.textColor
                font.pixelSize: TradesConfig.fontSize
                font.family: TradesConfig.fontFamily
            }

            // Оборот
            Text {
                text: turnoverStr
                width: tradeView.width * root.turnoverField
                color: TradesConfig.textColor
                font.pixelSize: TradesConfig.fontSize
                font.family: TradesConfig.fontFamily
            }

            // Время
            Text {
                text: timeStr
                width: tradeView.width * root.timeField
                color: TradesConfig.timeColor
                font.pixelSize: TradesConfig.fontSize
                font.family: TradesConfig.fontFamily
            }

        } // contentItem
    } // tradeView

} // root
