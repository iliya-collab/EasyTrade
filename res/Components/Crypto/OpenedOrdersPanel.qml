import QtQuick 2.15
import QtQuick.Layouts 2.15
import Application.Core 1.0
import Theme 1.0
import Components.Custom 1.0

Item {
    id: root

    readonly property var orderModel: OrderFilterProxyModel {
        sourceModel: AppCore.accountState.orders
        statuses: [EnumHelpers.OrderStatus.New, EnumHelpers.OrderStatus.PartiallyFilled]
    }
    readonly property int rowHeight: 20

    readonly property var columns: [
        { title: "Category",   width: 90  },
        { title: "Symbol",     width: 80  },
        { title: "Type",       width: 70  },
        { title: "Side",       width: 60  },
        { title: "Leaves/Qty", width: 90  },
        { title: "Price",      width: 80  },
        { title: "Order ID",   width: 120 },
        { title: "Created",    width: 100 }
    ]

    visible: orderModel !== null

    Component.onCompleted: console.info("OpenedOrdersPanel ready")

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.margins

        // Заголовок таблицы
        RowLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            spacing: 4

            Repeater {
                model: root.columns

                Text {
                    required property var modelData

                    text: modelData.title
                    color: "#aaaaaa"
                    font.pixelSize: Theme.fontSizeSmall
                    horizontalAlignment: Text.AlignLeft
                    elide: Text.ElideRight

                    Layout.fillWidth: true
                    Layout.preferredWidth: modelData.width
                    Layout.alignment: Qt.AlignVCenter
                }
            }
        }

        ListView {
            id: viewOrders
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            reuseItems: true
            model: root.orderModel
            delegate: OrderDelegate {
                width: viewOrders.width
                height: root.rowHeight
                columns: root.columns
            }
        }
    }

    // ---  OrderDelegate   ---
    component OrderDelegate: Item {
        id: orderDel

        property var columns: []

        required property string category
        required property string symbol
        required property string orderType
        required property string side
        required property string leavesQty
        required property string qty
        required property string price
        required property string orderId
        required property string createdTime

        function formatTime(ms)
        {
            const value = Number(ms)
            if (!value)
                return ""
            return Qt.formatDateTime(new Date(value), "dd.MM.yyyy HH:mm:ss")
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            spacing: 4

            Value { text: category;               Layout.preferredWidth: orderDel.columns[0].width }
            Value { text: symbol;                 Layout.preferredWidth: orderDel.columns[1].width }
            Value { text: orderType;              Layout.preferredWidth: orderDel.columns[2].width }
            Value {
                text: side
                color: side === "Buy" ? Theme.upColor : (side === "Sell" ? Theme.downColor : "#aaaaaa")
                Layout.preferredWidth: orderDel.columns[3].width
            }
            Value { text: leavesQty + "/" + qty;  Layout.preferredWidth: orderDel.columns[4].width }
            Value { text: price;                  Layout.preferredWidth: orderDel.columns[5].width }
            Value { text: orderId;                Layout.preferredWidth: orderDel.columns[6].width }
            Value {
                text: orderDel.formatTime(createdTime)
                Layout.preferredWidth: orderDel.columns[7].width
            }
        }

    }

    component Value: CustomText {
        color: "#aaaaaa"
        font.family: "monospace"
        font.pixelSize: Theme.fontSizeSmall
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        elide: Text.ElideRight
    }

}
