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

    // ==========================================================================================
    //  Логика определения hover-строки под курсором
    // ==========================================================================================
    function updateHover(absX, absY) {
        if (!bidsView || !asksView)
            return

        var bPos = bidsView.mapFromItem(root, absX, absY)
        var aPos = asksView.mapFromItem(root, absX, absY)

        var insideBids = bPos.x >= 0 && bPos.x < bidsView.width &&
                         bPos.y >= 0 && bPos.y < bidsView.height
        var insideAsks = aPos.x >= 0 && aPos.x < asksView.width &&
                         aPos.y >= 0 && aPos.y < asksView.height

        var bidIdx = insideBids
            ? bidsView.indexAt(bPos.x + bidsView.contentX, bPos.y + bidsView.contentY)
            : -1
        var askIdx = insideAsks
            ? asksView.indexAt(aPos.x + asksView.contentX, aPos.y + asksView.contentY)
            : -1

        if (bidIdx < 0 && askIdx < 0) {
            root.hoveredBidIndex = -1
            root.hoveredAskIndex = -1
            root.tooltipData = null
            return
        }

        if (bidIdx >= 0) {
            root.hoveredBidIndex = bidIdx
            root.hoveredAskIndex = -1
            root.tooltipData = bids.get(bidIdx)
            root.tooltipIsBid = true
        } else {
            root.hoveredBidIndex = -1
            root.hoveredAskIndex = askIdx
            root.tooltipData = asks.get(askIdx)
            root.tooltipIsBid = false
        }
    }

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
        } // asksView
    } // listsRow

    // ==========================================================================================
    //  Единый MouseArea поверх всего — перехватывает движение курсора
    // ==========================================================================================
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.NoButton

        onPositionChanged: function(mouse) {
            var p = mapToItem(root, mouse.x, mouse.y)
            root.mouseX = p.x
            root.mouseY = p.y
            root.updateHover(p.x, p.y)
        }

        onExited: {
            root.hoveredBidIndex = -1
            root.hoveredAskIndex = -1
            root.tooltipData = null
        }
    }

    // Tooltip
    Popup {
        id: tooltip

        width: 220
        height: tooltipText.implicitHeight + 16
        padding: 8
        modal: false
        focus: false
        closePolicy: Popup.NoAutoClose

        // Зазор между курсором и tooltip
        readonly property int gap: 14

        // Пытаемся рисовать справа от курсора; если не влезает — слева
        readonly property real desiredX: (root.mouseX + gap + width <= root.width)
                                         ? root.mouseX + gap
                                         : root.mouseX - gap - width

        // Пытаемся рисовать снизу от курсора; если не влезает — сверху
        readonly property real desiredY: (root.mouseY + gap + height <= root.height)
                                         ? root.mouseY + gap
                                         : root.mouseY - gap - height

        // Ограничиваем окном, но с учётом уже выбранной стороны
        x: Math.round(Math.max(5, Math.min(desiredX, root.width  - width  - 5)))
        y: Math.round(Math.max(5, Math.min(desiredY, root.height - height - 5)))

        visible: root.tooltipData !== null && root.tooltipData !== undefined

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
                return "Avg. Price:     " + formatNum(d.avgPrice) + "\n"
                     + "Total Volume:   " + formatNum(d.totalVolume) + "\n"
                     + "Total Turnover: " + formatNum(d.totalTurnover)
            }
        }
    } // tooltip

    function formatNum(v) {
        v = Number(v)
        if (v >= 1000000) return (v / 1000000).toFixed(2) + "M"
        if (v >= 1000)    return (v / 1000).toFixed(2) + "K"
        return v.toFixed(2)
    }

    // ==========================================================================================
    //  Делегат BIDS
    // ==========================================================================================
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
                elide: Text.ElideRight
            }
        }
    } // bidDel

    // ==========================================================================================
    //  Делегат ASKS
    // ==========================================================================================
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