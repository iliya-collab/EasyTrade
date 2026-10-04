import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls 2.15
import Application.Core 1.0

Item {
    id: root

    readonly property OrderbookSideModel asks: AppCore.marketState.asks ? AppCore.marketState.asks : null
    readonly property OrderbookSideModel bids: AppCore.marketState.bids ? AppCore.marketState.bids : null

    readonly property real maxVolume: {
        if (!asks || !bids)
            return 0
        return Math.max(asks.maxVolume, bids.maxVolume)
    }

    readonly property int rowHeight: 20

    property int hoveredBidIndex: -1
    property int hoveredAskIndex: -1

    property real mouseX: 0
    property real mouseY: 0
    property var tooltipData: null
    property bool tooltipIsBid: false

    visible: asks && bids

    // Заголовки
    Row {
        id: rowHeader
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 10
        height: 30

        Text {
            width: parent.width * 0.5
            text: "BIDS"
            color: "#00ff66"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: parent.width * 0.5
            text: "ASKS"
            color: "#ff4444"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    } // rowHeader

    // BIDS / ASKS
    Row {
        id: listsRow
        anchors.top: rowHeader.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.topMargin: 5
        anchors.bottomMargin: 10
        spacing: 0

        // BIDS
        ListView {
            id: bidsView
            width: parent.width / 2
            height: parent.height
            clip: true
            model: bids
            boundsBehavior: Flickable.StopAtBounds
            cacheBuffer: 0

            delegate: BidDelegate {
                width: bidsView.width
                height: root.rowHeight
                hoveredIndex: root.hoveredBidIndex
                maxVolume: root.maxVolume
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onPositionChanged: function(mouse) {
                    var p = mapToItem(root, mouse.x, mouse.y)
                    root.mouseX = p.x
                    root.mouseY = p.y

                    var idx = bidsView.indexAt(mouse.x + bidsView.contentX,
                                                mouse.y + bidsView.contentY)

                    if (idx >= 0) {
                        root.hoveredBidIndex = idx
                        root.hoveredAskIndex = -1
                        root.tooltipData = bids.get(idx)
                        root.tooltipIsBid = true
                    } else {
                        root.hoveredBidIndex = -1
                        root.tooltipData = null
                    }
                }
                onExited: {
                    root.hoveredBidIndex = -1
                    root.tooltipData = null
                }
            }
        } // bidsView

        // Разделитель
        Rectangle {
            width: 1
            height: parent.height
            color: "#4a4a6a"
        }

        // ASKS
        ListView {
            id: asksView
            width: parent.width / 2 - 1
            height: parent.height
            clip: true
            model: asks
            boundsBehavior: Flickable.StopAtBounds
            cacheBuffer: 0

            delegate: AskDelegate {
                width: asksView.width
                height: root.rowHeight
                hoveredIndex: root.hoveredAskIndex
                maxVolume: root.maxVolume
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onPositionChanged: function(mouse) {
                    var p = mapToItem(root, mouse.x, mouse.y)
                    root.mouseX = p.x
                    root.mouseY = p.y

                    var idx = asksView.indexAt(mouse.x + asksView.contentX,
                                                mouse.y + asksView.contentY)

                    if (idx >= 0) {
                        root.hoveredAskIndex = idx
                        root.hoveredBidIndex = -1
                        root.tooltipData = asks.get(idx)
                        root.tooltipIsBid = false
                    } else {
                        root.hoveredAskIndex = -1
                        root.tooltipData = null
                    }
                }
                onExited: {
                    root.hoveredAskIndex = -1
                    root.tooltipData = null
                }
            }
        } // asksView
    } // listsRow

    // Tooltip
    Popup {
        id: tooltip
        x: root.mouseX + 12
        y: root.mouseY - 10
        width: tooltipText.implicitWidth + 16
        height: tooltipText.implicitHeight + 16
        padding: 8
        modal: false
        focus: false
        closePolicy: Popup.NoAutoClose
        visible: root.tooltipData !== null
        background: Rectangle {
            color: Qt.rgba(30 / 255, 30 / 255, 40 / 255, 0.92)
            border.color: root.tooltipIsBid ? "#8bc34a" : "#ef5350"
            border.width: 1
            radius: 2
        }
        contentItem: Text {
            id: tooltipText
            color: "white"
            font.family: "monospace"
            font.pixelSize: 11
            text: {
                if (!root.tooltipData)
                    return ""
                var d = root.tooltipData
                return "Avg. Price:     " + Number(d.avgPrice).toFixed(2) + "\n"
                     + "Total Volume:   " + formatNum(d.totalVolume) + "\n"
                     + "Total Turnover: " + formatNum(d.totalTurnover)
            }
        }

        // Корректировка, чтобы не выходить за границы окна
        onXChanged: {
            if (x + width > root.width)
                x = root.mouseX - width - 12
            if (x < 0)
                x = 5
        }
        onYChanged: {
            if (y + height > root.height)
                y = root.height - height - 5
            if (y < 0)
                y = 5
        }
    } // tooltip

    function formatNum(v) {
        v = Number(v)
        if (v >= 1000000) return (v / 1000000).toFixed(2) + "M"
        if (v >= 1000)    return (v / 1000).toFixed(2) + "K"
        return v.toFixed(2)
    }

    // Делегат BIDS
    component BidDelegate: Item {
        id: bidDel
        required property int index
        required property real price
        required property real volume
        required property real turnover
        required property real avgPrice
        required property real totalVolume
        required property real totalTurnover

        property int hoveredIndex: -1
        property real maxVolume: 0

        // Градиентный бар объёма, растёт справа налево
        Rectangle {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height - 1
            width: bidDel.maxVolume > 0 ? parent.width * (bidDel.volume / bidDel.maxVolume) : 0
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop {
                    position: 0.0
                    color: index <= bidDel.hoveredIndex ? Qt.rgba(76 / 255, 175 / 255, 80 / 255, 0.10)
                                                          : Qt.rgba(76 / 255, 175 / 255, 80 / 255, 0.05)
                }
                GradientStop {
                    position: 1.0
                    color: index <= bidDel.hoveredIndex ? Qt.rgba(76 / 255, 175 / 255, 80 / 255, 0.80)
                                                          : Qt.rgba(76 / 255, 175 / 255, 80 / 255, 0.30)
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            visible: index <= bidDel.hoveredIndex
            color: Qt.rgba(139 / 255, 195 / 255, 74 / 255, 0.08)
            border.color: index === bidDel.hoveredIndex ? "#8bc34a" : "transparent"
            border.width: 1
        }

        // Тексты: Turnover 30% | Volume 30% | Price 40% (price прижат к центру)
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            spacing: 4

            Text {
                text: root.formatNum(bidDel.turnover)
                color: "#aaaaaa"
                font.family: "monospace"
                font.pixelSize: 11
                Layout.preferredWidth: parent.width * 0.30
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
            }
            Text {
                text: root.formatNum(bidDel.volume)
                color: "#ffffff"
                font.family: "monospace"
                font.pixelSize: 11
                Layout.preferredWidth: parent.width * 0.30
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
            }
            Text {
                text: bidDel.price.toFixed(2)
                color: "#8bc34a"
                font.family: "monospace"
                font.pixelSize: 11
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
                Layout.alignment: Qt.AlignVCenter
            }
        }
    } // bidDel

    // Делегат ASKS
    component AskDelegate: Item {
        id: askDel
        required property int index
        required property real price
        required property real volume
        required property real turnover
        required property real avgPrice
        required property real totalVolume
        required property real totalTurnover

        property int hoveredIndex: -1
        property real maxVolume: 0

        // Градиентный бар, растёт слева направо
        Rectangle {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height - 1
            width: askDel.maxVolume > 0 ? parent.width * (askDel.volume / askDel.maxVolume) : 0
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop {
                    position: 0.0
                    color: index <= askDel.hoveredIndex ? Qt.rgba(244 / 255, 67 / 255, 54 / 255, 0.80)
                                                          : Qt.rgba(244 / 255, 67 / 255, 54 / 255, 0.30)
                }
                GradientStop {
                    position: 1.0
                    color: index <= askDel.hoveredIndex ? Qt.rgba(244 / 255, 67 / 255, 54 / 255, 0.10)
                                                          : Qt.rgba(244 / 255, 67 / 255, 54 / 255, 0.05)
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            visible: index <= askDel.hoveredIndex
            color: Qt.rgba(244 / 255, 67 / 255, 54 / 255, 0.08)
            border.color: index === askDel.hoveredIndex ? "#ef5350" : "transparent"
            border.width: 1
        }

        // Тексты: Price 30% | Volume 30% | Turnover 40%
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            spacing: 4

            Text {
                text: askDel.price.toFixed(2)
                color: "#ef5350"
                font.family: "monospace"
                font.pixelSize: 11
                Layout.preferredWidth: parent.width * 0.30
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
            }
            Text {
                text: root.formatNum(askDel.volume)
                color: "#ffffff"
                font.family: "monospace"
                font.pixelSize: 11
                Layout.preferredWidth: parent.width * 0.30
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
            }
            Text {
                text: root.formatNum(askDel.turnover)
                color: "#aaaaaa"
                font.family: "monospace"
                font.pixelSize: 11
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignLeft
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
            }
        }
    } // askDel

}