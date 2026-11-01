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

    QtObject
    {
        id: internal

        readonly property var tradePairList: InstrumentFilterProxyModel {
            sourceModel: AppCore.marketState.instruments
            symbolFilter: ""
        }

        property Instrument currentInstrument
        property bool symbolSelected: false
        readonly property string currentSymbol: symbolSelected ? currentInstrument.symbol : ""

        function select(inst)
        {
            currentInstrument = inst
            symbolSelected = true
        }

        function confirm()
        {
            if (!symbolSelected)
                return

            AppCore.marketService.selectCurrentInstrument(currentInstrument)
            root.instrumentSelected(internal.currentInstrument)
            root.close()
        }
    }

    signal instrumentSelected(Instrument inst)

    ColumnLayout {
        id: contentLayout
        anchors.fill: parent
        anchors.margins: Theme.margins

        CustomTabBar {
            id: tabsBar
            Layout.fillWidth: true
            modelTabs: [
                { text: "spot",     type: EnumHelpers.MarketType.Spot },
                { text: "linear",   type: EnumHelpers.MarketType.Linear },
                { text: "inverse",  type: EnumHelpers.MarketType.Inverse },
                { text: "option",   type: EnumHelpers.MarketType.Option }
            ]
            onCurrentIndexChanged: {
                internal.symbolSelected = false
                AppCore.marketService.loadInstruments(modelTabs[currentIndex].type)
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
                text: tabsBar.modelTabs[tabsBar.currentIndex].text || "—"
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
                text: internal.currentSymbol || "—"
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

                delegate: Item {
                    id: lstItem
                    width: lstTrades.width
                    height: 34

                    required property string symbol
                    required property Instrument instrument
                    readonly property bool isSelected: internal.symbolSelected && internal.currentSymbol === symbol

                    Rectangle {
                        id: itemBg
                        anchors.fill: parent
                        radius: Theme.radius
                        color: lstItem.isSelected
                               ? Theme.selectColor
                               : (mouseArea.containsMouse ? Theme.hoverColor : "transparent")
                        border.color: lstItem.isSelected ? Theme.accentColor : "transparent"
                        border.width: lstItem.isSelected ? Theme.borderWidth : 0
                    }

                    Text {
                        anchors.fill: parent
                        anchors.leftMargin: Theme.padding
                        anchors.rightMargin: Theme.padding
                        text: lstItem.symbol
                        font.pixelSize: Theme.fontSizeBody
                        font.family: Theme.fontFamily
                        font.bold: lstItem.isSelected
                        color: lstItem.isSelected ? Theme.selectTextColor : Theme.textColor
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        acceptedButtons: Qt.LeftButton

                        onClicked: internal.select(lstItem.instrument)
                        onDoubleClicked: {
                            internal.select(lstItem.instrument)
                            internal.confirm()
                        }
                    }

                    Timer {
                        id: singleClickTimer
                        interval: 250
                        repeat: false
                        onTriggered: {}
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
                onClicked: internal.confirm()
            }
        }

    } // contentLayout

} // root
