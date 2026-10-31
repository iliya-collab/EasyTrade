import QtQuick
import QtQuick.Layouts
import Application.Core 1.0
import Components.Configs 1.0
import Components.Custom 1.0

Item {
    id: root

    property string selectedOrderId: ""
    property bool reversedList: true

    readonly property var orderModel: OrderFilterProxyModel {
        sourceModel: ReversedProxyModel {
            id: reversedModel
            sourceModel: AppCore.accountState.orders
            reverse: root.reversedList
        }
        statuses: [
            EnumHelpers.OrderStatus.Filled,
            EnumHelpers.OrderStatus.Cancelled,
            EnumHelpers.OrderStatus.Rejected
        ]
    }

    readonly property int rowHeight: OrdersConfig.rowHeight

    readonly property var columns: OrdersConfig.historicalColumns

    // Колонки для таблицы исполнений внутри раскрытой карточки
    readonly property var execColumns: OrdersConfig.executionColumns

    visible: orderModel !== null

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: OrdersConfig.margins

        CustomText {
            text: root.reversedList ? OrdersConfig.sortDescText : OrdersConfig.sortAscText
            color: OrdersConfig.sortArrowColor
            font.pixelSize: OrdersConfig.arrowFontSize
            MouseArea {
                anchors.fill: parent
                onClicked: root.reversedList = !root.reversedList
            }
        }

        // Заголовок таблицы
        RowLayout {
            Layout.fillWidth: true
            Layout.leftMargin: OrdersConfig.rowHorizontalMargin
            Layout.rightMargin: OrdersConfig.rowHorizontalMargin
            spacing: OrdersConfig.rowSpacing

            Repeater {
                model: root.columns

                Text {
                    required property var modelData

                    text: modelData.title
                    color: OrdersConfig.textSecondary
                    font.pixelSize: OrdersConfig.headerFontSize
                    horizontalAlignment: Text.AlignLeft
                    elide: Text.ElideRight

                    Layout.preferredWidth: modelData.width
                    Layout.alignment: Qt.AlignVCenter
                }
            }

            // распорка, чтобы таблица не «прилипала» влево
            Item { Layout.fillWidth: true }
        }

        ListView {
            id: viewOrders
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            reuseItems: false
            model: root.orderModel

            delegate: OrderDelegate {
                width: viewOrders.width
                columns: root.columns
                execColumns: root.execColumns
                expanded: root.selectedOrderId === orderId
                onClicked: root.selectedOrderId =
                    (root.selectedOrderId === orderId) ? "" : orderId
            }
        }
    }

    // --- OrderDelegate ---
    component OrderDelegate: Rectangle {
        id: orderDel

        property var columns: []
        property var execColumns: []
        property bool expanded: false

        signal clicked()

        required property string category
        required property string symbol
        required property string orderType
        required property string side
        required property string leavesQty
        required property string qty
        required property string price
        required property string orderId
        required property string status
        required property string createdTime

        readonly property int detailsPadding: OrdersConfig.detailsPadding

        height: root.rowHeight + (expanded ? detailsContent.implicitHeight + detailsPadding * 2 + OrdersConfig.detailsExtraHeight : 0)

        radius: OrdersConfig.radius
        color: "transparent"
        clip: true
        border.width: (orderDel.expanded || hoverHandler.hovered) ? OrdersConfig.borderWidth : 0
        border.color: orderDel.expanded ? OrdersConfig.borderExpandedColor : OrdersConfig.borderHoverColor

        function formatTime(ms) { return OrdersConfig.formatTime(ms) }

        Behavior on color { ColorAnimation { duration: OrdersConfig.colorAnimMs } }
        Behavior on height { NumberAnimation { duration: OrdersConfig.heightAnimMs } }

        HoverHandler { id: hoverHandler }

        MouseArea {
            height: root.rowHeight
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            acceptedButtons: Qt.LeftButton
            onClicked: orderDel.clicked()
        }

        // Основная строка ордера
        RowLayout {
            id: mainRow
            height: root.rowHeight
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.leftMargin: OrdersConfig.rowHorizontalMargin
            anchors.rightMargin: OrdersConfig.rowHorizontalMargin
            spacing: OrdersConfig.rowSpacing

            Value { text: category;               Layout.preferredWidth: orderDel.columns[0].width }
            Value { text: symbol;                 Layout.preferredWidth: orderDel.columns[1].width }
            Value { text: orderType;              Layout.preferredWidth: orderDel.columns[2].width }
            Value {
                text: side
                color: OrdersConfig.sideColor(side)
                Layout.preferredWidth: orderDel.columns[3].width
            }
            Value { text: leavesQty + "/" + qty;  Layout.preferredWidth: orderDel.columns[4].width }
            Value { text: price;                  Layout.preferredWidth: orderDel.columns[5].width }
            Value { text: orderId;                Layout.preferredWidth: orderDel.columns[6].width }
            Value { text: status;                 Layout.preferredWidth: orderDel.columns[7].width }
            Value {
                text: orderDel.formatTime(createdTime)
                Layout.preferredWidth: orderDel.columns[8].width
            }

            Item { Layout.fillWidth: true }
        } // mainRow

        // Раскрывающийся блок
        Item {
            id: detailsBlock
            anchors.top: mainRow.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: orderDel.expanded ? detailsContent.implicitHeight + detailsPadding * 2 + OrdersConfig.detailsExtraHeight : 0
            opacity: orderDel.expanded ? 1.0 : 0.0
            visible: height > 0

            Behavior on opacity { NumberAnimation { duration: OrdersConfig.opacityAnimMs } }

            Rectangle {
                anchors.fill: parent
                anchors.margins: OrdersConfig.detailsInnerMargin
                color: OrdersConfig.detailsBackground
                bottomLeftRadius: OrdersConfig.radius
                bottomRightRadius: OrdersConfig.radius
                clip: true

                Flickable {
                    anchors.fill: parent
                    anchors.margins: detailsPadding
                    contentWidth: width
                    contentHeight: detailsContent.implicitHeight
                    clip: true

                    ColumnLayout {
                        id: detailsContent
                        width: parent.width
                        spacing: OrdersConfig.detailsSpacing

                        CustomText {
                            text: OrdersConfig.executionsTitlePrefix + orderDel.orderId
                            color: OrdersConfig.detailsTitleColor
                            font.pixelSize: OrdersConfig.detailsFontSize
                        }

                        // Шапка таблицы исполнений
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: OrdersConfig.detailsSpacing

                            Repeater {
                                model: orderDel.execColumns

                                Cell {
                                    required property var modelData
                                    text: modelData.title
                                    colWidth: modelData.width
                                }
                            }

                            Item { Layout.fillWidth: true }
                        }

                        // Строки исполнений
                        ListView {
                            id: execList
                            Layout.fillWidth: true
                            Layout.preferredHeight: contentHeight
                            implicitHeight: contentHeight
                            interactive: false
                            clip: true

                            model: ExecutionFilterProxyModel {
                                sourceModel: AppCore.accountState.executions
                                orderId: orderDel.orderId
                            }

                            delegate: RowLayout {
                                width: execList.width
                                spacing: OrdersConfig.detailsSpacing

                                Cell {
                                    text: execId
                                    colWidth: orderDel.execColumns[0].width
                                }
                                Cell {
                                    text: OrdersConfig.formatTime(execTime)
                                    colWidth: orderDel.execColumns[1].width
                                }
                                Cell {
                                    text: execQty
                                    colWidth: orderDel.execColumns[2].width
                                }
                                Cell {
                                    text: execPrice
                                    colWidth: orderDel.execColumns[3].width
                                }
                                Cell {
                                    text: execFee
                                    colWidth: orderDel.execColumns[4].width
                                }

                                Item { Layout.fillWidth: true }
                            }
                        }
                    }
                }
            }
        } // detailsBlock
    }

    // --- Ячейка таблицы ордеров ---
    component Value: CustomText {
        color: OrdersConfig.textSecondary
        font.family: OrdersConfig.cellFontFamily
        font.pixelSize: OrdersConfig.cellFontSize
        Layout.alignment: Qt.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        elide: Text.ElideRight
    }

    // --- Ячейка таблицы исполнений (с фиксированной шириной колонки) ---
    component Cell: CustomText {
        property real colWidth: 0

        color: OrdersConfig.textSecondary
        font.family: OrdersConfig.cellFontFamily
        font.pixelSize: OrdersConfig.cellFontSize
        Layout.preferredWidth: colWidth
        Layout.alignment: Qt.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        elide: Text.ElideRight
    }
}