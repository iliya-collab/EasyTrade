import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import Theme 1.0
import Application.Core 1.0
import Components.Custom 1.0

Popup {
    id: root

    signal hoveredChanged(bool hovered)

    QtObject
    {
        id: internal
        readonly property var wallet: AppCore.accountState.walletController
        readonly property tInstrumentInfo inst: AppCore.marketState.current.info
        readonly property string coin: inst ? inst.baseCoin : ""
        property tAssetInfo asset: wallet.assets.asset(coin)

        function refresh() { asset = wallet.assets.asset(coin) }
        onCoinChanged: refresh()

        readonly property Connections conn: Connections {
            target: internal.wallet.assets
            function onDataChanged()  { internal.refresh() }
            function onRowsInserted() { internal.refresh() }
            function onRowsRemoved()  { internal.refresh() }
            function onModelReset()   { internal.refresh() }
        }

    }

    component StatRow: RowLayout {
        property string label
        property string value
        property color valueColor: Theme.textColor
        property bool shown: true
        visible: shown
        Layout.fillWidth: true
        CustomLabel {
            text: label;
            opacity: 0.7
        }
        Item { Layout.fillWidth: true }
        CustomLabel {
            text: value;
            color: valueColor
        }
    }

    x: parent ? parent.width - width : 0
    y: parent ? parent.height + 4 : 0

    width: 300
    //height: 420

    modal: false
    focus: false
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    background: Rectangle {
        color: Theme.windowColor
        border.color: Theme.borderColor
        border.width: Theme.borderWidth
        radius: Theme.radius

        HoverHandler {
            id: hoverHandler
            onHoveredChanged: root.hoveredChanged(hoverHandler.hovered)
        }

    }

    contentItem: ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.margins
        spacing: 8

        CustomLabel { text: qsTr("Wallet") }

        Rectangle {
            id: assetState
            Layout.fillWidth: true
            implicitHeight: coinCol.implicitHeight + 2 * Theme.margins
            radius: Theme.radius
            color:Theme.panelColor
            border.width: Theme.borderWidth
            border.color: Theme.borderColor

            ColumnLayout {
                id: coinCol
                anchors.fill: parent
                anchors.margins: Theme.margins

                CustomLabel {
                    font.pixelSize: Theme.fontSizeHeader
                    text: Format.fmt(internal.asset.walletBalance) + " " + internal.coin
                }
                CustomLabel {
                    font.pixelSize: Theme.fontSizeBody
                    text: "≈ " + Format.fmt(internal.asset.usdValue, 2) + " USD"
                }

                StatRow {
                    label: qsTr("Equity")
                    value: Format.fmt(internal.asset.equity)
                }
                StatRow {
                    label: qsTr("Unrealised PnL")
                    value: Format.fmt(internal.asset.unrealisedPnl)
                    valueColor: Format.pnlColor(internal.asset.unrealisedPnl)
                }
                StatRow {
                    label: qsTr("Realised PnL")
                    value: Format.fmt(internal.asset.cumRealisedPnl)
                    valueColor: Format.pnlColor(internal.asset.cumRealisedPnl)
                }
                StatRow {
                    label: qsTr("In orders")
                    value: Format.fmt(internal.asset.totalOrderIM)
                    shown: Format.num(internal.asset.totalOrderIM) !== 0
                }
                StatRow {
                    label: qsTr("In positions")
                    value: Format.fmt(internal.asset.totalPositionIM)
                    shown: Format.num(internal.asset.totalPositionIM) !== 0
                }
                StatRow {
                    label: qsTr("Locked")
                    value: Format.fmt(internal.asset.locked)
                    shown: Format.num(internal.asset.locked) !== 0
                }
                StatRow {
                    label: qsTr("Borrowed")
                    value: Format.fmt(internal.asset.spotBorrow)
                    shown: Format.num(internal.asset.spotBorrow) !== 0
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            implicitHeight: accCol.implicitHeight + 2 * Theme.margins
            radius: Theme.radius;
            color: Theme.panelColor
            border.width: Theme.borderWidth;
            border.color: Theme.borderColor

            ColumnLayout {
                id: accCol
                anchors.fill: parent; anchors.margins: Theme.margins
                StatRow {
                    label: qsTr("Total equity")
                    value: Format.fmt(internal.wallet.totalEquity, 2) + " USD"
                }
                StatRow {
                    label: qsTr("Available")
                    value: Format.fmt(internal.wallet.totalAvailableBalance, 2) + " USD"
                }
                StatRow {
                    label: qsTr("Perp UPL")
                    value: Format.fmt(internal.wallet.totalPerpUPL, 2) + " USD"
                    valueColor: Format.pnlColor(internal.wallet.totalPerpUPL)
                }
                StatRow {
                    label: qsTr("Initial margin rate")
                    value: Format.pct(internal.wallet.accountIMRate)
                }
                StatRow {
                    label: qsTr("Maintenance margin rate")
                    value: Format.pct(internal.wallet.accountMMRate)
                }
            }
        }

        Item { Layout.fillHeight: true }
    }

}
