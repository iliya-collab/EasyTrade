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
                        SplitView.preferredWidth: chartSplit.width * 0.90
                        SplitView.minimumWidth: chartSplit.width * 0.70
                        currentTrade: AppCore.marketState.currentSymbol
                    }

                    Rectangle {
                        color: Theme.windowColor
                        SplitView.fillHeight: true
                        SplitView.fillWidth: true
                        SplitView.minimumWidth: 350
                        clip: true

                        MarketDataWidget {
                            anchors.fill: parent
                            anchors.margins: Theme.margins
                        }
                    }
                } // chartSplit
            } // topContent

            Rectangle {
                id: bottomContent
                SplitView.fillHeight: true
                SplitView.minimumHeight: 200
                color: Theme.windowColor
                clip: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.margins

                    CustomTabBar {
                        modelTabs: [
                            { text: "Opened orders" },
                            { text: "Positons" },
                            { text: "Historical orders" },
                            { text: "History of trade" }
                        ]
                        onCurrentIndexChanged: loader.sourceComponent = loader.components[currentIndex]
                    }

                    Loader {
                        id: loader
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        property var components: [
                            openedOrdersComp,
                            positionsComp,
                            historyOrdersComp,
                            tradeHistoryComp
                        ]
                    }

                    Component {
                        id: openedOrdersComp

                        OpenedOrdersWidget {
                            id: openedOrders
                            Connections {
                                target: openedOrders
                                ignoreUnknownSignals: true

                                function onClickedCancelOrder(category, symbol, orderId, orderLinkId) {
                                    AppCore.accountService.cancelOrder({
                                        "category": category,
                                        "symbol": symbol,
                                        "orderId": orderId,
                                        "orderLinkId": orderLinkId
                                    })
                                }

                                function onClickedAmendOrder(category, symbol, orderType, orderId, orderLinkId) {
                                    amendDialog.category = category
                                    amendDialog.symbol = symbol
                                    amendDialog.orderType = orderType
                                    amendDialog.orderId = orderId
                                    amendDialog.orderLinkId = orderLinkId
                                    amendDialog.open()
                                }

                                function onClickedCancelAllOrder() {
                                    AppCore.accountService.cancelAllOrders({
                                        "category":  EnumHelpers.marketTypeToApiString(AppCore.marketState.currentCategory),
                                        "symbol": AppCore.marketState.currentSymbol
                                    })
                                }
                            }
                        } // openedOrders
                    }
                    Component { id: positionsComp; PositionsWidget {} }
                    Component { id: historyOrdersComp; HistoricalOrdersWidget {} }
                    Component { id: tradeHistoryComp; TradeHistoryWidget {} }

                }
            } // bottomContent

        } // mainLayout
    }
} // TradePage
