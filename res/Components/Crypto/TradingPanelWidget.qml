import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtCore
import Application.Core 1.0
import Components.Crypto 1.0
import Components.Custom 1.0
import Theme 1.0

Rectangle {
    id: root
    color: Theme.windowColor
    clip: true

    // Высота узкой полоски в свёрнутом состоянии
    readonly property int collapsedHeight: 28

    property alias hidden: store.hidden

    signal amendOrderRequested(string category, string symbol, string orderType,
                               string orderId, string orderLinkId)

    Settings {
        id: store
        category: "TradingPanelWidget"
        property bool hidden: false
    }

    // === Содержимое ===
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.margins
        visible: !root.hidden

        RowLayout {
            Layout.fillWidth: true

            CustomTabBar {
                modelTabs: [
                    { text: "Opened orders" },
                    { text: "Positons" },
                    { text: "Historical orders" },
                    { text: "History of trade" }
                ]
                onCurrentIndexChanged: loader.sourceComponent = loader.components[currentIndex]
            }

            Item { Layout.fillWidth: true }

            ToolButton {
                text: "\u25BE"
                onClicked: root.hidden = true
            }
        }

        Loader {
            id: loader
            Layout.fillHeight: true
            Layout.fillWidth: true
            active: !root.hidden

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
                        root.amendOrderRequested(category, symbol, orderType, orderId, orderLinkId)
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

    // === Свёрнутая полоска: клик возвращает панель ===
    ToolButton {
        anchors.fill: parent
        visible: root.hidden
        text: "\u25B4  " + qsTr("Orders")
        onClicked: root.hidden = false
    }

    // === Контекстное меню ===
    TapHandler {
        acceptedButtons: Qt.RightButton
        onTapped: panelMenu.popup(point.position.x, point.position.y)
    }

    Menu {
        id: panelMenu
        background: Rectangle {
            implicitWidth: 180
            color: Theme.windowColor
            border.width: Theme.borderWidth
            border.color: Theme.borderColor
        }

        MenuItem {
            text: root.hidden ? qsTr("Show") : qsTr("Hide")
            onTriggered: root.hidden = !root.hidden
        }
    }
}
