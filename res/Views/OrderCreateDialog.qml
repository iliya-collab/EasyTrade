import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Theme 1.0
import Components.Custom 1.0
import Application.Core 1.0

Dialog {
    id: root

    // ---- Размеры ----
    width: 420
    height: advancedOpen ? 560 : 360

    modal: true
    parent: Overlay.overlay
    anchors.centerIn: Overlay.overlay
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    // ---- Контекст ордера ----
    property int category: AppCore.marketState.currentCategory
    property string symbol: AppCore.marketState.currentSymbol

    // ---- Состояние формы ----
    property int side: EnumHelpers.OrderSide.Buy
    property int orderType: EnumHelpers.OrderType.Limit
    property bool advancedOpen: false
    property bool reduceOnlyEnabled: false

    readonly property bool isLimit: orderType === EnumHelpers.OrderType.Limit
    readonly property bool isSpot: category === EnumHelpers.MarketType.Spot

    readonly property real qtyValue: Number(qtyField.text)
    readonly property real priceValue: Number(priceField.text)

    readonly property bool qtyValid: qtyField.text.length > 0 && qtyValue > 0
    readonly property bool priceValid: !isLimit || (priceField.text.length > 0 && priceValue > 0)
    readonly property bool formValid: qtyValid && priceValid

    readonly property real estimatedTotal: (isLimit && qtyValid && priceValid) ? qtyValue * priceValue : 0

    // ---- Фон диалога ----
    background: Rectangle {
        color: Theme.windowColor
        border.color: Theme.borderColor
        border.width: Theme.borderWidth
        radius: Theme.radius
    }

    // ---- Заголовок ----
    header: Item {
        implicitHeight: headerRow.implicitHeight + 16

        RowLayout {
            id: headerRow
            anchors.fill: parent
            anchors.margins: 8

            CustomText {
                text: root.symbol
                font.family: "monospace"
                font.pixelSize: Theme.fontSizeHeader
                font.bold: true
            }

            CustomText {
                text: "Create order"
                color: "#888888"
                font.pixelSize: Theme.fontSizeSmall
            }

            Item { Layout.fillWidth: true }

            Rectangle {
                color: "#2a2a3a"
                radius: 3
                implicitWidth: catLabel.implicitWidth + 12
                implicitHeight: 20
                CustomText {
                    id: catLabel
                    anchors.centerIn: parent
                    text: EnumHelpers.marketTypeToApiString
                          ? EnumHelpers.marketTypeToApiString(root.category)
                          : root.category
                    color: "#aaaaaa"
                    font.family: "monospace"
                    font.pixelSize: Theme.fontSizeSmall
                }
            }
        }
    }

    // ---- Контент ----
    contentItem: Flickable {
        id: flick
        contentWidth: width
        contentHeight: content.height
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        ColumnLayout {
            id: content
            width: flick.width
            spacing: 14

            // ---- Buy / Sell ----
            RowLayout {
                Layout.fillWidth: true
                spacing: 2

                CustomButton {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 36
                    text: "BUY"
                    checkable: true
                    autoExclusive: true
                    checked: root.side === EnumHelpers.OrderSide.Buy
                    onClicked: root.side = EnumHelpers.OrderSide.Buy
                    background: Rectangle {
                        color: parent.checked ? "#4caf50" : "#2a2a3a"
                        radius: 4
                    }
                    contentItem: Text {
                        text: parent.text
                        color: parent.checked ? "#0d1a0d" : "#8bc34a"
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                CustomButton {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 36
                    text: "SELL"
                    checkable: true
                    autoExclusive: true
                    checked: root.side === EnumHelpers.OrderSide.Sell
                    onClicked: root.side = EnumHelpers.OrderSide.Sell
                    background: Rectangle {
                        color: parent.checked ? "#ef5350" : "#2a2a3a"
                        radius: 4
                    }
                    contentItem: Text {
                        text: parent.text
                        color: parent.checked ? "#2a0d0d" : "#ef5350"
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            // ---- Тип ордера ----
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                CustomText { text: "Order type"; color: "#888888"; font.pixelSize: Theme.fontSizeSmall }
                CustomComboBox {
                    Layout.fillWidth: true
                    model: ["Limit", "Market"]
                    currentIndex: root.isLimit ? 0 : 1
                    onActivated: function(idx) {
                        root.orderType = idx === 0
                            ? EnumHelpers.OrderType.Limit
                            : EnumHelpers.OrderType.Market
                    }
                }
            }

            // ---- Цена ----
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                visible: root.isLimit
                CustomText { text: "Price"; color: "#888888"; font.pixelSize: Theme.fontSizeSmall }
                CustomTextField {
                    id: priceField
                    Layout.fillWidth: true
                    placeholderText: "0.00"
                    validator: DoubleValidator { bottom: 0; decimals: 8; notation: DoubleValidator.StandardNotation }
                    font.family: "monospace"
                    background: Rectangle {
                        color: "#242430"
                        radius: 4
                        border.color: priceField.activeFocus ? "#8bc34a" : "#3a3a4a"
                        border.width: 1
                    }
                }
            }

            // ---- Количество ----
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                CustomText { text: "Quantity"; color: "#888888"; font.pixelSize: Theme.fontSizeSmall }
                CustomTextField {
                    id: qtyField
                    Layout.fillWidth: true
                    placeholderText: "0.000"
                    validator: DoubleValidator { bottom: 0; decimals: 8; notation: DoubleValidator.StandardNotation }
                    font.family: "monospace"
                    background: Rectangle {
                        color: "#242430"
                        radius: 4
                        border.color: qtyField.activeFocus ? "#8bc34a" : "#3a3a4a"
                        border.width: 1
                    }
                }
            }

            // ---- Est. total ----
            RowLayout {
                Layout.fillWidth: true
                visible: root.isLimit && root.estimatedTotal > 0
                CustomText { text: "Est. total:"; color: "#888888"; font.pixelSize: Theme.fontSizeSmall }
                Item { Layout.fillWidth: true }
                CustomText {
                    text: root.estimatedTotal.toFixed(2)
                    font.family: "monospace"
                    font.pixelSize: Theme.fontSizeSmall
                }
            }

            // ---- Разделитель + Advanced ----
            Rectangle { Layout.fillWidth: true; height: 1; color: "#3a3a4a" }

            RowLayout {
                Layout.fillWidth: true
                CustomText {
                    text: root.advancedOpen ? "▾ Advanced" : "▸ Advanced"
                    color: "#8bc34a"
                    font.pixelSize: Theme.fontSizeBody
                    MouseArea {
                        anchors.fill: parent
                        onClicked: root.advancedOpen = !root.advancedOpen
                    }
                }
                Item { Layout.fillWidth: true }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 10
                visible: root.advancedOpen

                RowLayout {
                    Layout.fillWidth: true
                    CustomSwitch { id: orderLinkIdCheck; text: "Order link ID" }
                    CustomTextField {
                        id: orderLinkIdField
                        Layout.fillWidth: true
                        enabled: orderLinkIdCheck.checked
                        placeholderText: "name"
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    CustomText { text: "Time in force"; color: "#888888"; font.pixelSize: Theme.fontSizeSmall }
                    CustomComboBox {
                        id: tifBox
                        Layout.fillWidth: true
                        model: tifModel
                        valueRole: "value"
                        textRole: "label"
                        currentIndex: 0
                    }
                }

                CustomSwitch {
                    id: reduceOnlyBox
                    visible: !root.isSpot
                    text: "Reduce only"
                    checked: false
                    contentItem: Text {
                        text: reduceOnlyBox.text
                        color: "#cccccc"
                        leftPadding: reduceOnlyBox.indicator.width + 6
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    CustomSwitch { id: tpCheck; text: "Take profit" }
                    CustomTextField {
                        id: tpField
                        Layout.fillWidth: true
                        enabled: tpCheck.checked
                        placeholderText: "price"
                        validator: DoubleValidator { bottom: 0; decimals: 8 }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    CustomSwitch { id: slCheck; text: "Stop loss" }
                    CustomTextField {
                        id: slField
                        Layout.fillWidth: true
                        enabled: slCheck.checked
                        placeholderText: "price"
                        validator: DoubleValidator { bottom: 0; decimals: 8 }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    CustomSwitch { id: triggerCheck; text: "Trigger price" }
                    CustomTextField {
                        id: triggerField
                        Layout.fillWidth: true
                        enabled: triggerCheck.checked
                        placeholderText: "price"
                        validator: DoubleValidator { bottom: 0; decimals: 8 }
                    }
                }
            }

            Item { Layout.preferredHeight: 4 }

            // ---- Кнопки ----
            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                CustomButton {
                    Layout.fillWidth: true
                    text: "Cancel"
                    onClicked: root.close()
                }
                CustomButton {
                    Layout.fillWidth: true
                    text: root.side === EnumHelpers.OrderSide.Buy
                          ? "Buy " + root.symbol
                          : "Sell " + root.symbol
                    enabled: root.formValid
                    onClicked: root.submitOrder()
                    background: Rectangle {
                        color: !parent.enabled ? "#3a3a4a"
                             : (root.side === EnumHelpers.OrderSide.Buy ? "#4caf50" : "#ef5350")
                        radius: 4
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "#ffffff"
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }

    // ---- Модель ----
    ListModel {
        id: tifModel
        ListElement { label: "GTC";      value: EnumHelpers.TimeInForce.GTC }
        ListElement { label: "IOC";      value: EnumHelpers.TimeInForce.IOC }
        ListElement { label: "FOK";      value: EnumHelpers.TimeInForce.FOK }
        ListElement { label: "PostOnly"; value: EnumHelpers.TimeInForce.PostOnly }
    }

    function submitOrder() {
        if (!formValid)
            return

        AppCore.accountService.createOrder({
            "category": EnumHelpers.marketTypeToApiString(root.category),
            "symbol": root.symbol,
            "side": EnumHelpers.orderSideToString(root.side),
            "orderType": EnumHelpers.orderTypeToString(root.orderType),
            "qty": root.qtyValue,
            "price": root.priceValue,
            "takeProfit": tpField.text,
            "stopLoss": slField.text,
            "triggerPrice": triggerField.text,
            "reduceOnly": reduceOnlyBox.checked,
            "orderLinkId": orderLinkIdField.text,
            "timeInForce": EnumHelpers.timeInForceToString(tifBox.currentValue),
        })

        root.close()
    }
}