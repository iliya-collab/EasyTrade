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
        id: internal

        property bool isComponentReady: false
        readonly property var tradePairList: TradePairsFilterProxyModel
        {
            sourceModel: AppCore.marketState.tradePairs
            symbolFilter: ""
        }
        property var currentCategory: null
        property var currentSymbol: null
        property bool symbolSelected: currentSymbol !== null && currentSymbol !== ""
    }

    signal tradeSelected(string symbol)

    ColumnLayout {
        id: contentLayout
        anchors.fill: parent
        anchors.margins: Theme.margins

        CustomTabBar {
            id: tabsBar
            Layout.fillWidth: true
            modelTabs: [
                { text: "spot",     type: Tools.MarketType.Spot },
                { text: "linear",   type: Tools.MarketType.Linear },
                { text: "inverse",  type: Tools.MarketType.Inverse },
                { text: "option",   type: Tools.MarketType.Option }
            ]
            onCurrentIndexChanged: {
                internal.currentCategory = modelTabs[currentIndex].text
                internal.currentSymbol = null
                AppCore.marketService.loadTradePairs(modelTabs[currentIndex].type)
            }
        }

        CustomTextField {
            id: txtFilter
            Layout.fillWidth: true
            placeholderText: "Enter symbol..."
            onTextChanged: internal.tradePairList.symbolFilter = txtFilter.text
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.margins

            Text {
                text: "Category:"
                font.pixelSize: Theme.fontSizeBody
                font.family: Theme.fontFamily
                color: Theme.textColor
            }
            Text {
                text: internal.currentCategory ?? "—"
                font.pixelSize: Theme.fontSizeBody
                font.family: Theme.fontFamily
                font.bold: true
                color: Theme.accentColor
            }

            Item { Layout.fillWidth: true }

            Text {
                text: "Symbol:"
                font.pixelSize: Theme.fontSizeBody
                font.family: Theme.fontFamily
                color: Theme.textColor
            }
            Text {
                text: internal.currentSymbol ?? "—"
                font.pixelSize: Theme.fontSizeBody
                font.family: Theme.fontFamily
                font.bold: true
                color: Theme.accentColor
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            border.color: Theme.borderColor
            border.width: Theme.borderWidth
            color: "transparent"
            radius: Theme.radius

            ListView {
                id: lstTrades
                model: internal.tradePairList
                anchors.fill: parent
                clip: true

                delegate: ItemDelegate {
                    id: lstItem
                    width: lstTrades.width
                    text: symbol

                    property string symbolName: symbol
                    readonly property bool isSelected: internal.currentSymbol === symbolName

                    background: Rectangle {
                        color: lstItem.isSelected
                               ? Theme.selectColor
                               : (lstItem.pressed ? Theme.pressColor
                               : (lstItem.hovered ? Theme.hoverColor : "transparent"))
                        radius: Theme.radius
                        border.color: lstItem.isSelected ? Theme.accentColor : "transparent"
                        border.width: lstItem.isSelected ? Theme.borderWidth : 0
                    }

                    contentItem: Text {
                        text: lstItem.text
                        font.pixelSize: Theme.fontSizeBody
                        font.family: Theme.fontFamily
                        font.bold: lstItem.isSelected
                        leftPadding: Theme.padding
                        color: lstItem.isSelected ? Theme.selectTextColor : Theme.textColor
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        console.info("clicked symbol:", symbol, "type:", typeof symbol)
                        internal.currentSymbol = symbol
                        console.info("currentSymbol now:", internal.currentSymbol)
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

        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.margins

            Item { Layout.fillWidth: true }

            CustomButton {
                text: "Cancel"
                implicitHeight: 30
                onClicked: root.close()
            }

            CustomButton {
                text: "Select"
                enabled: internal.symbolSelected
                implicitHeight: 30
                onClicked: {
                    root.tradeSelected(internal.currentSymbol)
                    root.close()
                }
            }
        }

    } // contentLayout

} // root
