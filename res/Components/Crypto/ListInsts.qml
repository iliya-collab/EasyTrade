import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Application.Core 1.0
import Components.Custom 1.0
import Theme 1.0

Rectangle {
    id: root

    //Layout.fillWidth: true
    //Layout.fillHeight: true
    border.color: Theme.borderColor
    border.width: Theme.borderWidth
    color: "transparent"
    radius: Theme.radius

    signal clickedOnInstrument(tInstrumentInfo inst)
    signal doubleClickedOnInstrument(tInstrumentInfo inst)

    property alias listInsts: modelInsts.sourceModel
    property alias symbolFilter: modelInsts.symbolFilter
    property alias categoryFilter: modelInsts.categoryFilter
    property string selectedId: ""

    readonly property var insts: InstrumentFilterProxyModel {
        id: modelInsts
        symbolFilter: ""
        categoryFilter: EnumHelpers.MarketType.Unknown
    }


    ListView {
        id: lstTrades
        model: insts
        anchors.fill: parent
        clip: true

        delegate: Item {
            id: lstItem
            width: lstTrades.width
            height: 34

            required property string symbol
            required property tInstrumentInfo instrument
            readonly property string currentId: instrument.id
            readonly property bool isSelected: root.selectedId === currentId

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

                onClicked: clickedOnInstrument(lstItem.instrument)
                onDoubleClicked: doubleClickedOnInstrument(lstItem.instrument)
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
}
