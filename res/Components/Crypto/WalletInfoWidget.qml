import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Application.Core 1.0
import Components.Custom 1.0
import Theme 1.0

Rectangle {
    id: root
    color: "transparent"
    clip: true

    readonly property var walletData: AppCore.accountState.walletController

    // Строка "название — значение"
    component StatRow: RowLayout {
        property string label
        property string value
        property color  valueColor: Theme.textColor
        property bool   shown: true
        visible: shown
        Layout.fillWidth: true
        spacing: Theme.spacing

        CustomLabel {
            text: label
            opacity: 0.7
            font.pixelSize: Theme.fontSizeBody
            color: Theme.textColor
        }
        Item { Layout.fillWidth: true }
        CustomLabel {
            text: value
            font.pixelSize: Theme.fontSizeBody
            color: valueColor
        }
    }

    // ------------------------------------------------------------------
    //  Подробная информация об активе (один попап на весь виджет)
    // ------------------------------------------------------------------
    function openDetails(coin, anchorItem) {
        detailsPopup.coin = coin
        detailsPopup.refresh()
        const p = anchorItem.mapToItem(root, 0, anchorItem.height + 2)
        detailsPopup.x = Math.max(0, Math.min(p.x + anchorItem.width - detailsPopup.width,
                                              root.width - detailsPopup.width))
        detailsPopup.y = Math.max(0, Math.min(p.y, root.height - detailsPopup.implicitHeight))
        detailsPopup.open()
    }

    Popup {
        id: detailsPopup
        parent: root
        width: Math.min(300, root.width)
        padding: Theme.padding
        modal: false
        focus: false
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        property string coin: ""
        property tAssetInfo asset: root.walletData.assets.asset(coin)
        function refresh() { asset = root.walletData.assets.asset(coin) }

        // Данные приходят по WS — обновляем открытый попап вживую
        Connections {
            target: root.walletData.assets
            enabled: detailsPopup.visible
            function onDataChanged()  { detailsPopup.refresh() }
            function onRowsRemoved()  { detailsPopup.refresh() }
            function onModelReset()   { detailsPopup.refresh() }
        }

        background: Rectangle {
            color: Theme.windowColor
            border.color: Theme.borderColor
            border.width: Theme.borderWidth
            radius: Theme.radius
        }

        contentItem: ColumnLayout {
            spacing: Theme.spacing / 2

            RowLayout {
                Layout.fillWidth: true
                CustomLabel {
                    text: detailsPopup.coin
                    font.pixelSize: Theme.fontSizeHeader
                    font.bold: true
                    color: Theme.accentColor
                }
                Item { Layout.fillWidth: true }
                ToolButton {
                    text: "×"
                    onClicked: detailsPopup.close()
                }
            }

            Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: Theme.borderColor }

            // Баланс
            StatRow { label: qsTr("Balance");        value: Format.fmt(detailsPopup.asset.walletBalance, 6) }
            StatRow { label: qsTr("Equity");         value: Format.fmt(detailsPopup.asset.equity, 6) }
            StatRow { label: qsTr("Value, USD");     value: Format.fmt(detailsPopup.asset.usdValue, 2) }

            // PnL
            StatRow { label: qsTr("Unrealised PnL"); value: Format.fmt(detailsPopup.asset.unrealisedPnl, 6)
                      valueColor: Format.pnlColor(detailsPopup.asset.unrealisedPnl) }
            StatRow { label: qsTr("Realised PnL");   value: Format.fmt(detailsPopup.asset.cumRealisedPnl, 6)
                      valueColor: Format.pnlColor(detailsPopup.asset.cumRealisedPnl) }

            // Маржа и блокировки — только если есть что показать
            StatRow { label: qsTr("Order IM");       value: Format.fmt(detailsPopup.asset.totalOrderIM, 6)
                      shown: Format.num(detailsPopup.asset.totalOrderIM) !== 0 }
            StatRow { label: qsTr("Position IM");    value: Format.fmt(detailsPopup.asset.totalPositionIM, 6)
                      shown: Format.num(detailsPopup.asset.totalPositionIM) !== 0 }
            StatRow { label: qsTr("Position MM");    value: Format.fmt(detailsPopup.asset.totalPositionMM, 6)
                      shown: Format.num(detailsPopup.asset.totalPositionMM) !== 0 }
            StatRow { label: qsTr("Locked (spot)");  value: Format.fmt(detailsPopup.asset.locked, 6)
                      shown: Format.num(detailsPopup.asset.locked) !== 0 }

            // Займы
            StatRow { label: qsTr("Spot borrow");    value: Format.fmt(detailsPopup.asset.spotBorrow, 6)
                      shown: Format.num(detailsPopup.asset.spotBorrow) !== 0 }
            StatRow { label: qsTr("Borrow total");   value: Format.fmt(detailsPopup.asset.borrowAmount, 6)
                      shown: Format.num(detailsPopup.asset.borrowAmount) !== 0 }
            StatRow { label: qsTr("Accrued interest"); value: Format.fmt(detailsPopup.asset.accruedInterest, 8)
                      shown: Format.num(detailsPopup.asset.accruedInterest) !== 0 }

            // Прочее
            StatRow { label: qsTr("Spot hedging qty"); value: Format.fmt(detailsPopup.asset.spotHedgingQty, 6)
                      shown: Format.num(detailsPopup.asset.spotHedgingQty) !== 0 }
            StatRow { label: qsTr("Bonus");          value: Format.fmt(detailsPopup.asset.bonus, 6)
                      shown: Format.num(detailsPopup.asset.bonus) !== 0 }
            StatRow { label: qsTr("Margin collateral"); value: detailsPopup.asset.marginCollateral ? qsTr("Yes") : qsTr("No") }
            StatRow { label: qsTr("Collateral switch"); value: detailsPopup.asset.collateralSwitch ? qsTr("On") : qsTr("Off") }
        }
    }

    // ------------------------------------------------------------------
    //  Основной layout
    // ------------------------------------------------------------------
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.padding
        spacing: Theme.spacing / 2

        // ---------- Информация о кошельке ----------
        CustomLabel {
            text: qsTr("Wallet") + (walletData.accountType ? " (" + walletData.accountType + ")" : "")
            font.pixelSize: Theme.fontSizeBody
            font.bold: true
            color: Theme.accentColor
        }

        Rectangle {
            Layout.fillWidth: true
            implicitHeight: walletCol.implicitHeight + Theme.padding * 2
            radius: Theme.radius
            color: Theme.panelColor
            border.width: Theme.borderWidth
            border.color: Theme.borderColor

            ColumnLayout {
                id: walletCol
                anchors.fill: parent
                anchors.margins: Theme.padding
                spacing: Theme.spacing / 2

                StatRow { label: qsTr("Total equity");    value: Format.fmt(walletData.totalEquity, 2) + " USD" }
                StatRow { label: qsTr("Wallet balance");  value: Format.fmt(walletData.totalWalletBalance, 2) + " USD" }
                StatRow { label: qsTr("Available");       value: Format.fmt(walletData.totalAvailableBalance, 2) + " USD" }
                StatRow { label: qsTr("Perp UPL");        value: Format.fmt(walletData.totalPerpUPL, 2) + " USD"
                          valueColor: Format.pnlColor(walletData.totalPerpUPL) }
                StatRow { label: qsTr("Initial margin");  value: Format.fmt(walletData.totalInitialMargin, 2) + " USD" }
                StatRow { label: qsTr("Maint. margin");   value: Format.fmt(walletData.totalMaintenanceMargin, 2) + " USD" }
                StatRow { label: qsTr("IM rate");         value: Format.pct(walletData.accountIMRate) }
                StatRow { label: qsTr("MM rate");         value: Format.pct(walletData.accountMMRate)
                          valueColor: Format.riskColor(walletData.accountMMRate) }
            }
        }

        // ---------- Активы ----------
        CustomLabel {
            text: qsTr("Assets")
            font.pixelSize: Theme.fontSizeBody
            font.bold: true
            color: Theme.accentColor
        }

        // Ширины колонок считаются от ширины самого виджета
        QtObject {
            id: cols
            readonly property real spacing: Theme.spacing * 3
            readonly property real btnWidth: 28
            readonly property real total: Math.max(root.width - Theme.padding * 2 - spacing - btnWidth, 240)
            readonly property real coinWidth:          total * 0.30
            readonly property real walletBalanceWidth: total * 0.35
            readonly property real usdValueWidth:      total * 0.35
        }

        // Заголовки
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacing

            CustomLabel {
                text: qsTr("Coin")
                Layout.preferredWidth: cols.coinWidth
                Layout.fillWidth: true
                Layout.minimumWidth: 0
                elide: Text.ElideRight
                font.pixelSize: Theme.fontSizeBody
                font.bold: true
                color: Theme.textColor
            }
            CustomLabel {
                text: qsTr("Balance")
                Layout.preferredWidth: cols.walletBalanceWidth
                Layout.fillWidth: true
                Layout.minimumWidth: 0
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
                font.pixelSize: Theme.fontSizeBody
                font.bold: true
                color: Theme.textColor
            }
            CustomLabel {
                text: qsTr("USD")
                Layout.preferredWidth: cols.usdValueWidth
                Layout.fillWidth: true
                Layout.minimumWidth: 0
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
                font.pixelSize: Theme.fontSizeBody
                font.bold: true
                color: Theme.textColor
            }
            Item { Layout.preferredWidth: cols.btnWidth }   // место под кнопку деталей
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: Theme.borderColor
        }

        ListView {
            id: viewAssets
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            reuseItems: true
            model: walletData.assets

            delegate: RowLayout {
                width: viewAssets.width
                spacing: Theme.spacing

                CustomLabel {
                    text: model.coin
                    Layout.preferredWidth: cols.coinWidth
                    Layout.fillWidth: true
                    Layout.minimumWidth: 0
                    elide: Text.ElideRight
                    font.pixelSize: Theme.fontSizeBody
                    color: Theme.textColor
                }
                CustomLabel {
                    text: Format.fmt(model.walletBalance, 2)
                    Layout.preferredWidth: cols.walletBalanceWidth
                    Layout.fillWidth: true
                    Layout.minimumWidth: 0
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignRight
                    font.pixelSize: Theme.fontSizeBody
                    color: Theme.textColor
                }
                CustomLabel {
                    text: Format.fmt(model.usdValue, 2)
                    Layout.preferredWidth: cols.usdValueWidth
                    Layout.fillWidth: true
                    Layout.minimumWidth: 0
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignRight
                    font.pixelSize: Theme.fontSizeBody
                    color: Theme.textColor
                }
                ToolButton {
                    id: infoBtn
                    text: "i"
                    Layout.preferredWidth: cols.btnWidth
                    Layout.preferredHeight: cols.btnWidth
                    padding: 0
                    onClicked: root.openDetails(model.coin, infoBtn)

                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Asset details")
                    ToolTip.delay: 500
                }
            }
        }
    }
}
