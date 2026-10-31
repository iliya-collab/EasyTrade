import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Application.Core 1.0
import Components.Custom 1.0
import Theme 1.0

Dialog {
    id: root

    // --- Входные данные (заполняются перед open()) ---
    property string category: ""
    property string symbol: ""
    property string orderId: ""
    property string orderLinkId: ""

    // Текущие значения (для префилла)
    property string currentQty: ""
    property string currentPrice: ""
    property string currentTriggerPrice: ""
    property string currentTakeProfit: ""
    property string currentStopLoss: ""

    property string orderType: ""

    signal amendRequested(var request)

    title: qsTr("Amend order")
    modal: true
    anchors.centerIn: Overlay.overlay
    width: 420
    padding: Theme.margins

    background: Rectangle {
        color: Theme.windowColor
        border.color: Theme.borderColor
        border.width: Theme.borderWidth
        radius: Theme.radius
    }

    readonly property string orderRef:
        (orderId && orderId.length > 0) ? orderId
        : (orderLinkId && orderLinkId.length > 0) ? orderLinkId
        : "—"

    readonly property bool isMarket: orderType.toLowerCase() === EnumHelpers.orderTypeToString(EnumHelpers.OrderType.Market)

    contentItem: ColumnLayout {
        spacing: Theme.spacing

        // --- Информация об ордере ---
        GridLayout {
            Layout.fillWidth: true
            columns: 2
            columnSpacing: Theme.spacing
            rowSpacing: 4

            CustomText {
                text: qsTr("Symbol:")
                color: "#aaaaaa"
                font.pixelSize: Theme.fontSizeSmall
            }
            CustomText {
                text: root.symbol
                font.pixelSize: Theme.fontSizeSmall
            }

            CustomText {
                text: qsTr("Category:")
                color: "#aaaaaa"
                font.pixelSize: Theme.fontSizeSmall
            }
            CustomText {
                text: root.category
                font.pixelSize: Theme.fontSizeSmall
            }

            CustomText {
                text: qsTr("Type:")
                color: "#aaaaaa"
                font.pixelSize: Theme.fontSizeSmall
            }
            CustomText {
                text: root.orderType
                font.pixelSize: Theme.fontSizeSmall
            }

            CustomText {
                text: qsTr("Order ID:")
                color: "#aaaaaa"
                font.pixelSize: Theme.fontSizeSmall
            }
            CustomText {
                text: root.orderRef
                font.pixelSize: Theme.fontSizeSmall
            }
        }

        Rectangle { Layout.fillWidth: true; height: 1; color: Theme.borderColor }

        // --- Поля для изменения ---

        // Qty — показываем всегда (для Market тоже можно менять объём)
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2

            CustomText {
                text: qsTr("Quantity")
                color: "#aaaaaa"
                font.pixelSize: Theme.fontSizeSmall
            }
            CustomTextField {
                id: qtyField
                Layout.fillWidth: true
                placeholderText: root.currentQty
                text: ""
                font.pixelSize: Theme.fontSizeSmall
                validator: DoubleValidator { bottom: 0; decimals: 8; notation: DoubleValidator.StandardNotation }
            }
        }

        // Price — только для Limit
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2
            visible: !root.isMarket

            CustomText {
                text: qsTr("Price")
                color: "#aaaaaa"
                font.pixelSize: Theme.fontSizeSmall
            }
            CustomTextField {
                id: priceField
                Layout.fillWidth: true
                placeholderText: root.currentPrice
                text: ""
                font.pixelSize: Theme.fontSizeSmall
                validator: DoubleValidator { bottom: 0; decimals: 8; notation: DoubleValidator.StandardNotation }
            }
        }

        // Trigger price — для стоп/триггер ордеров (Limit)
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2
            visible: !root.isMarket

            CustomText {
                text: qsTr("Trigger price")
                color: "#aaaaaa"
                font.pixelSize: Theme.fontSizeSmall
            }
            CustomTextField {
                id: triggerPriceField
                Layout.fillWidth: true
                placeholderText: root.currentTriggerPrice
                text: ""
                font.pixelSize: Theme.fontSizeSmall
                validator: DoubleValidator { bottom: 0; decimals: 8; notation: DoubleValidator.StandardNotation }
            }
        }

        // Take profit / Stop loss
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacing

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2
                CustomText {
                    text: qsTr("Take Profit")
                    color: "#aaaaaa"
                    font.pixelSize: Theme.fontSizeSmall
                }
                CustomTextField {
                    id: takeProfitField
                    Layout.fillWidth: true
                    placeholderText: root.currentTakeProfit
                    text: ""
                    font.pixelSize: Theme.fontSizeSmall
                    validator: DoubleValidator { bottom: 0; decimals: 8; notation: DoubleValidator.StandardNotation }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2
                CustomText {
                    text: qsTr("Stop Loss")
                    color: "#aaaaaa"
                    font.pixelSize: Theme.fontSizeSmall
                }
                CustomTextField {
                    id: stopLossField
                    Layout.fillWidth: true
                    placeholderText: root.currentStopLoss
                    text: ""
                    font.pixelSize: Theme.fontSizeSmall
                    validator: DoubleValidator { bottom: 0; decimals: 8; notation: DoubleValidator.StandardNotation }
                }
            }
        }

        // --- Кнопки ---
        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: Theme.spacing
            spacing: Theme.spacing

            Item { Layout.fillWidth: true }

            CustomButton {
                text: qsTr("Cancel")
                font.pixelSize: Theme.fontSizeSmall
                onClicked: root.reject()
            }

            CustomButton {
                text: qsTr("Apply")
                font.pixelSize: Theme.fontSizeSmall
                enabled: qtyField.text.length > 0
                      || priceField.text.length > 0
                      || triggerPriceField.text.length > 0
                      || takeProfitField.text.length > 0
                      || stopLossField.text.length > 0
                onClicked: root.submit()
            }
        }
    }

    function submit()
    {
        var req = {
            "category": root.category,
            "symbol":   root.symbol
        }

        if (root.orderId && root.orderId.length > 0)
            req.orderId = root.orderId
        if (root.orderLinkId && root.orderLinkId.length > 0)
            req.orderLinkId = root.orderLinkId

        if (qtyField.text.length > 0)          req.qty = qtyField.text
        if (priceField.text.length > 0)        req.price = priceField.text
        if (triggerPriceField.text.length > 0) req.triggerPrice = triggerPriceField.text
        if (takeProfitField.text.length > 0)   req.takeProfit = takeProfitField.text
        if (stopLossField.text.length > 0)     req.stopLoss = stopLossField.text

        root.amendRequested(req)
        root.accept()
    }

    // Сброс полей при каждом открытии
    onOpened: {
        qtyField.text = ""
        priceField.text = ""
        triggerPriceField.text = ""
        takeProfitField.text = ""
        stopLossField.text = ""
    }
}