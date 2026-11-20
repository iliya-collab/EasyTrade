import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Application.Core 1.0
import Components.Crypto 1.0
import Components.Custom 1.0
import Theme 1.0

Item {
    id: root

    Layout.fillWidth: true
    Layout.fillHeight: true

    AmendOrderDialog {
        id: amendDialog

        onAmendRequested: function(request) {
            AppCore.accountService.amendOrder(request)
        }
    }

    Item {
        anchors.fill: parent

        SplitView {
            id: mainLayout
            anchors.fill: parent
            anchors.margins: Theme.margins
            orientation: Qt.Vertical

            handle: Rectangle {
                implicitHeight: 6
                color: SplitHandle.pressed ? Theme.accentColor
                     : SplitHandle.hovered ? Qt.lighter(Theme.borderColor, 1.5)
                     : "transparent"

                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width
                    height: 1
                    color: Theme.borderColor
                }
            }

            ColumnLayout {
                id: topContent
                SplitView.preferredHeight: mainLayout.height * 0.80
                SplitView.fillHeight: bottomPanel.hidden
                SplitView.minimumHeight: mainLayout.height * 0.50
                clip: true

                Rectangle {
                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true
                    color: Theme.windowColor
                    clip: true

                    implicitHeight: tickerWidget.implicitHeight

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 1
                        spacing: Theme.spacing

                        TickerWidget {
                            id: tickerWidget
                            Layout.fillWidth: true
                        }
                    }
                } // Rectangle

                SplitView {
                    id: chartSplit
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    orientation: Qt.Horizontal

                    handle: Rectangle {
                        implicitWidth: 6
                        color: SplitHandle.pressed ? Theme.accentColor
                             : SplitHandle.hovered ? Qt.lighter(Theme.borderColor, 1.5)
                             : "transparent"

                        Rectangle {
                            anchors.centerIn: parent
                            width: 1
                            height: parent.height
                            color: Theme.borderColor
                        }
                    }

                    KlineWidget {
                        id: candleChart
                        SplitView.fillHeight: true
                        SplitView.fillWidth: orderFlowWidget.hidden
                        SplitView.preferredWidth: chartSplit.width * 0.90
                        SplitView.minimumWidth: chartSplit.width * 0.70
                    }

                    OrderFlowWidget {
                        id: orderFlowWidget
                        SplitView.fillHeight: true
                        SplitView.fillWidth: !hidden
                        SplitView.minimumWidth: hidden ? collapsedWidth : 350
                        SplitView.maximumWidth: hidden ? collapsedWidth : Number.POSITIVE_INFINITY
                    }
                } // chartSplit
            } // topContent

            TradingWidget {
                id: bottomPanel

                SplitView.fillHeight: !hidden
                SplitView.minimumHeight: hidden ? collapsedHeight : 200
                SplitView.maximumHeight: hidden ? collapsedHeight : Number.POSITIVE_INFINITY

                onAmendOrderRequested: function(category, symbol, orderType, orderId, orderLinkId) {
                    amendDialog.category = category
                    amendDialog.symbol = symbol
                    amendDialog.orderType = orderType
                    amendDialog.orderId = orderId
                    amendDialog.orderLinkId = orderLinkId
                    amendDialog.open()
                }
            } // bottomPanel

        } // mainLayout
    }
} // TradePage
