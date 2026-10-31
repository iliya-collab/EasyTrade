import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Components.Configs 1.0

Item {
    id: root

    property real percentAsk: 0
    property real percentBid: 0

    RowLayout {
        anchors.fill: parent

        // Индикатор Bid
        Rectangle {
            width: indicatorBid.contentHeight + SentimentConfig.markerPadding
            height: indicatorBid.contentHeight + SentimentConfig.markerPadding
            Layout.alignment: Qt.AlignVCenter
            color: SentimentConfig.markerBackground
            border.color: SentimentConfig.bidColor
            border.width: SentimentConfig.markerBorderWidth
            topLeftRadius: SentimentConfig.markerRadius
            bottomLeftRadius: SentimentConfig.markerRadius
            Text {
                id: indicatorBid
                anchors.centerIn: parent
                text: SentimentConfig.bidLabel
                font.pixelSize: SentimentConfig.fontSize
                font.family: SentimentConfig.fontFamily
                color: SentimentConfig.bidColor
            } // Text
        }

        // Центральня полоса
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Row {
                anchors.fill: parent
                spacing: 0

                Rectangle {
                    id: bidSide
                    height: parent.height
                    width: parent.width * root.percentBid / 100
                    color: SentimentConfig.bidColor
                    Text {
                        anchors.left: parent.left
                        anchors.leftMargin: SentimentConfig.textMargin
                        anchors.verticalCenter: parent.verticalCenter
                        text: SentimentConfig.formatPercent(root.percentBid)
                        color: SentimentConfig.barTextColor
                        font.pixelSize: SentimentConfig.fontSize
                        font.family: SentimentConfig.fontFamily
                    }
                }

                Rectangle {
                    id: askSide
                    height: parent.height
                    width: parent.width * root.percentAsk / 100
                    color: SentimentConfig.askColor
                    Text {
                        anchors.right: parent.right
                        anchors.rightMargin: SentimentConfig.textMargin
                        anchors.verticalCenter: parent.verticalCenter
                        text: SentimentConfig.formatPercent(root.percentAsk)
                        color: SentimentConfig.barTextColor
                        font.pixelSize: SentimentConfig.fontSize
                        font.family: SentimentConfig.fontFamily
                    }
                }
            } // Row
        } // Item

        // Индикатор Ask
        Rectangle {
            width: indicatorAsk.contentHeight + SentimentConfig.markerPadding
            height: indicatorAsk.contentHeight + SentimentConfig.markerPadding
            Layout.alignment: Qt.AlignVCenter
            color: SentimentConfig.markerBackground
            border.color: SentimentConfig.askColor
            border.width: SentimentConfig.markerBorderWidth
            topRightRadius: SentimentConfig.markerRadius
            bottomRightRadius: SentimentConfig.markerRadius
            Text {
                id: indicatorAsk
                anchors.centerIn: parent
                text: SentimentConfig.askLabel
                font.pixelSize: SentimentConfig.fontSize
                font.family: SentimentConfig.fontFamily
                color: SentimentConfig.askColor
            } // Text
        }

    }

}
