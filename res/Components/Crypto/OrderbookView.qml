import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls 2.15
import Application.Core 1.0
import Components.Configs 1.0

Item {
    id: root

    readonly property OrderbookSideModel asks: AppCore.marketState.current.asks ? AppCore.marketState.current.asks : null
    readonly property OrderbookSideModel bids: AppCore.marketState.current.bids ? AppCore.marketState.current.bids : null

    readonly property real maxVolume: {
        if (!asks || !bids)
            return 0
        return Math.max(asks.maxVolume, bids.maxVolume)
    }

    readonly property int rowHeight: OrderbookConfig.rowHeight

    property int hoveredBidIndex: -1
    property int hoveredAskIndex: -1

    property real mouseX: 0
    property real mouseY: 0
    property var tooltipData: null
    property bool tooltipIsBid: false

    visible: asks && bids

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
        anchors.margins: OrderbookConfig.margins
        height: OrderbookConfig.headerHeight

        Text {
            width: parent.width * 0.5
            text: "BIDS"
            color: OrderbookConfig.headerBidColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            width: parent.width * 0.5
            text: "ASKS"
            color: OrderbookConfig.headerAskColor
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
        anchors.leftMargin: OrderbookConfig.margins
        anchors.rightMargin: OrderbookConfig.margins
        anchors.topMargin: OrderbookConfig.listsTopMargin
        anchors.bottomMargin: OrderbookConfig.margins
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
            width: OrderbookConfig.separatorWidth
            height: parent.height
            color: OrderbookConfig.separatorColor
        }

        // ASKS
        ListView {
            id: asksView
            width: parent.width / 2 - OrderbookConfig.separatorWidth
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

        width: OrderbookConfig.tooltipWidth
        height: tooltipText.implicitHeight + 2 * padding
        padding: OrderbookConfig.tooltipPadding
        modal: false
        focus: false
        closePolicy: Popup.NoAutoClose

        // Зазор между курсором и tooltip
        readonly property int gap: OrderbookConfig.tooltipGap

        // Пытаемся рисовать справа от курсора; если не влезает — слева
        readonly property real desiredX: (root.mouseX + gap + width <= root.width)
                                         ? root.mouseX + gap
                                         : root.mouseX - gap - width

        // Пытаемся рисовать снизу от курсора; если не влезает — сверху
        readonly property real desiredY: (root.mouseY + gap + height <= root.height)
                                         ? root.mouseY + gap
                                         : root.mouseY - gap - height

        // Ограничиваем окном, но с учётом уже выбранной стороны
        x: Math.round(Math.max(OrderbookConfig.tooltipEdgeMargin, Math.min(desiredX, root.width  - width  - OrderbookConfig.tooltipEdgeMargin)))
        y: Math.round(Math.max(OrderbookConfig.tooltipEdgeMargin, Math.min(desiredY, root.height - height - OrderbookConfig.tooltipEdgeMargin)))

        visible: root.tooltipData !== null && root.tooltipData !== undefined

        background: Rectangle {
            color: OrderbookConfig.tooltipBg
            border.color: root.tooltipIsBid ? OrderbookConfig.bidColor : OrderbookConfig.askColor
            border.width: 1
            radius: OrderbookConfig.tooltipRadius
        }

        contentItem: Text {
            id: tooltipText
            color: OrderbookConfig.textPrimary
            font.family: OrderbookConfig.fontFamily
            font.pixelSize: OrderbookConfig.fontSize
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
        return OrderbookConfig.formatNumber(v)
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
                    color: OrderbookConfig.withAlpha(OrderbookConfig.bidFill, index <= bidDel.hoveredIndex ? OrderbookConfig.barAlphaFaintHover
                                                                                : OrderbookConfig.barAlphaFaint)
                }
                GradientStop {
                    position: 1.0
                    color: OrderbookConfig.withAlpha(OrderbookConfig.bidFill, index <= bidDel.hoveredIndex ? OrderbookConfig.barAlphaStrongHover
                                                                                : OrderbookConfig.barAlphaStrong)
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            visible: index <= bidDel.hoveredIndex
            color: OrderbookConfig.withAlpha(OrderbookConfig.bidHoverBg, OrderbookConfig.hoverRowAlpha)
            border.color: index === bidDel.hoveredIndex ? OrderbookConfig.bidColor : "transparent"
            border.width: 1
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: OrderbookConfig.rowHorizontalMargin
            anchors.rightMargin: OrderbookConfig.rowHorizontalMargin
            spacing: OrderbookConfig.rowSpacing

            Text {
                text: root.formatNum(bidDel.turnover)
                color: OrderbookConfig.textSecondary
                font.family: OrderbookConfig.fontFamily
                font.pixelSize: OrderbookConfig.fontSize
                Layout.preferredWidth: parent.width * OrderbookConfig.columnRatio
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
            }
            Text {
                text: root.formatNum(bidDel.volume)
                color: OrderbookConfig.textPrimary
                font.family: OrderbookConfig.fontFamily
                font.pixelSize: OrderbookConfig.fontSize
                Layout.preferredWidth: parent.width * OrderbookConfig.columnRatio
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
            }
            Text {
                text: OrderbookConfig.formatPrice(bidDel.price)
                color: OrderbookConfig.bidColor
                font.family: OrderbookConfig.fontFamily
                font.pixelSize: OrderbookConfig.fontSize
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
                    color: OrderbookConfig.withAlpha(OrderbookConfig.askFill, index <= askDel.hoveredIndex ? OrderbookConfig.barAlphaStrongHover
                                                                                : OrderbookConfig.barAlphaStrong)
                }
                GradientStop {
                    position: 1.0
                    color: OrderbookConfig.withAlpha(OrderbookConfig.askFill, index <= askDel.hoveredIndex ? OrderbookConfig.barAlphaFaintHover
                                                                                : OrderbookConfig.barAlphaFaint)
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            visible: index <= askDel.hoveredIndex
            color: OrderbookConfig.withAlpha(OrderbookConfig.askHoverBg, OrderbookConfig.hoverRowAlpha)
            border.color: index === askDel.hoveredIndex ? OrderbookConfig.askColor : "transparent"
            border.width: 1
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: OrderbookConfig.rowHorizontalMargin
            anchors.rightMargin: OrderbookConfig.rowHorizontalMargin
            spacing: OrderbookConfig.rowSpacing

            Text {
                text: OrderbookConfig.formatPrice(askDel.price)
                color: OrderbookConfig.askColor
                font.family: OrderbookConfig.fontFamily
                font.pixelSize: OrderbookConfig.fontSize
                Layout.preferredWidth: parent.width * OrderbookConfig.columnRatio
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
            }
            Text {
                text: root.formatNum(askDel.volume)
                color: OrderbookConfig.textPrimary
                font.family: OrderbookConfig.fontFamily
                font.pixelSize: OrderbookConfig.fontSize
                Layout.preferredWidth: parent.width * OrderbookConfig.columnRatio
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
            }
            Text {
                text: root.formatNum(askDel.turnover)
                color: OrderbookConfig.textSecondary
                font.family: OrderbookConfig.fontFamily
                font.pixelSize: OrderbookConfig.fontSize
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignLeft
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
            }
        }
    } // askDel

}