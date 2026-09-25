import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Application.Core 1.0
import Components.Custom 1.0
import Theme 1.0

Window {

    id: root

    width: 320
    height: 420
    minimumWidth: 280
    minimumHeight: 320
    visible: true
    title: "Select trade symbol"
    color: Theme.windowColor

    QtObject {
        id: state

        property bool isComponentReady: false
        readonly property var tradePairList: TradePairsFilterProxyModel
        {
            sourceModel: AppCore.marketState.tradePairs
            quoteCoinFilter: ""
        }
    }

    signal tradeSelected(string symbol)

    ColumnLayout {
        id: contentLayout
        anchors.fill: parent
        anchors.margins: Theme.margins

        CustomTabBar {
            id: marketTabsBar
            modelTabs: [
                { text: "spot",     type: Tools.MarketType.Spot },
                { text: "linear",   type: Tools.MarketType.Linear },
                { text: "inverse",  type: Tools.MarketType.Inverse },
                { text: "option",   type: Tools.MarketType.Option }
            ]
            onCurrentIndexChanged: {
                var curType = modelTabs[currentIndex].type
                AppCore.marketService.loadTradePairs(curType)
            }
        } // tabsBar

        CustomTabBar {
            id: tabsBar
            modelTabs: [
                { text: "ALL" },
                { text: "USDT" },
                { text: "USDC" },
                { text: "USDE" }
            ]
            onCurrentIndexChanged: {
                var curText = modelTabs[currentIndex].text
                state.tradePairList.quoteCoinFilter = curText === "ALL" ? "" : curText
            }
        } // tabsBar

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            border.color: Theme.borderColor
            border.width: Theme.borderWidth
            color: "transparent"
            radius: Theme.radius

            ListView {
                id: lstTrades
                model: state.tradePairList
                anchors.fill: parent
                clip: true

                delegate: ItemDelegate {
                    id: lstItem
                    width: lstTrades.width
                    text: symbol
                    background: Rectangle {
                        color: lstItem.pressed ? Theme.pressColor : (lstItem.hovered ? Theme.hoverColor : "transparent")
                    }
                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: Theme.fontSizeBody
                        font.family: Theme.fontFamily
                        padding: Theme.padding
                        color: Theme.textColor
                    }
                    onClicked: {
                        root.tradeSelected(symbol)
                        root.close()
                    }
                }

                ScrollBar.vertical: ScrollBar {
                    id: scrollBar
                    policy: ScrollBar.AsNeeded
                    minimumSize: Theme.scrollBarMinSize
                    background: Rectangle {
                        color: Theme.scrollBarColor
                        border.color: Theme.borderColor
                        border.width: Theme.borderWidth
                        radius: Theme.radius
                    }
                    contentItem: Rectangle {
                        implicitWidth: Theme.scrollBarWidth
                        implicitHeight: Theme.scrollBarHeight
                        color: scrollBar.hovered ? Theme.hoverColor : Theme.sliderColor
                        border.color: Theme.borderColor
                        border.width: Theme.borderWidth
                        radius: Theme.radius
                    }
                }
            } // lstTrades
        } // Rectangle
    } // contentLayout

} // root
