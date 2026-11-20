import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Application.UI 1.0
import Application.Core 1.0
import Theme 1.0
import Components.Custom 1.0
import Components.Crypto 1.0

// Главное окно
ApplicationWindow {

    id: mainWindow
    visible: true
    minimumWidth: 1000
    minimumHeight: 600
    width: 1500
    height: 800
    title: "EasyTrade"
    color: Theme.windowColor

    //visibility: "FullScreen"

    Loader {
        id: settingsWindowLoader
        active: false
        source: "Views/SettingsWindow.qml"

        Connections {
            target: settingsWindowLoader.item
            ignoreUnknownSignals: true
            function onVisibleChanged() {
                if (settingsWindowLoader.item && !settingsWindowLoader.item.visible)
                    settingsWindowLoader.active = false
            }
        }
    }

    Loader {
        id: selecterTradeWindowLoader
        active: false
        source: "Views/InstrumentSelectionWindow.qml"

        onLoaded: {
            item.currentInstrumentSelected.connect(function(inst) {
                AppCore.marketService.selectCurrentInstrument(inst)
                AppCore.accountService.requestFeeRates({
                    "category": EnumHelpers.marketTypeToApiString(inst.category),
                    "symbol": inst.symbol
                })
                AppCore.accountService.requestOrderHistory({
                    "category": EnumHelpers.marketTypeToApiString(inst.category) })
                AppCore.accountService.requestOpenOrders({
                    "category": EnumHelpers.marketTypeToApiString(inst.category) })
                AppCore.accountService.requestTradeHistory({
                    "category": EnumHelpers.marketTypeToApiString(inst.category) })
            })
            item.instrumentOpened.connect(function(inst) {
                AppCore.marketService.openInstrument(inst)
            })
            item.instrumentClosed.connect(function(inst) {
                AppCore.marketService.closeInstrument(inst)
            })
        }

        Connections {
            target: selecterTradeWindowLoader.item
            ignoreUnknownSignals: true
            function onVisibleChanged() {
                if (selecterTradeWindowLoader.item && !selecterTradeWindowLoader.item.visible)
                    selecterTradeWindowLoader.active = false
            }
        }
    }

    Loader {
        id: logWindowLoader
        active: false
        source: "Views/LogWindow.qml"

        Connections {
            target: logWindowLoader.item
            ignoreUnknownSignals: true
            function onVisibleChanged() {
                if (logWindowLoader.item && !logWindowLoader.item.visible)
                    Qt.callLater(function() { logWindowLoader.active = false })
            }
        }
    }

    OrderCreateDialog {
        id: orderDialog
    }

    ExportDialog {
        id: klinesExportDialog
        title: qsTr("Export klines")
        defaultName: "klines"

        GridLayout {
            columns: 2
            Layout.fillWidth: true
            CustomLabel { text: qsTr("Interval") }
            CustomComboBox { id: kInterval; model: ["1","3","5","15","30","60","120","240","360","720","D","W","M"] }
            CustomLabel { text: qsTr("From") }
            CustomTextField { id: kStart; placeholderText: "2026-01-01 00:00"; Layout.fillWidth: true }
            CustomLabel { text: qsTr("To") }
            CustomTextField { id: kEnd; placeholderText: "2026-01-31 23:59"; Layout.fillWidth: true }
        }

        onExportRequested: function(path, format, opts) {
            AppCore.marketService.exportKlines(path, format, opts, {
                "category": EnumHelpers.marketTypeToApiString(
                                AppCore.marketState.current.info.category),
                "symbol":   AppCore.marketState.current.info.symbol,
                "interval": kInterval.currentText,
                "start":    Date.parse(kStart.text),
                "end":      Date.parse(kEnd.text)
            })
        }
    }

    ExportDialog {
        id: tradesExportDialog
        title: qsTr("Export trades")
        defaultName: "trades"

        onExportRequested: function(path, format, opts) {
            AppCore.marketService.exportTrades(path, format, opts, {
                "category": EnumHelpers.marketTypeToApiString(
                               AppCore.marketState.current.info.category),
                "symbol":   AppCore.marketState.current.info.symbol
            })
        }
    }

    ExportDialog {
        id: ordersExportDialog
        title: qsTr("Export orders")
        defaultName: "orders"

        onExportRequested: function(path, format, opts) {
            AppCore.accountService.exportOrders(path, format, opts, {
                "category": EnumHelpers.marketTypeToApiString(
                                AppCore.marketState.current.info.category),
                "symbol":   AppCore.marketState.current.info.symbol
            })
        }
    }

    ExportDialog {
        id: execsExportDialog
        title: qsTr("Export execs")
        defaultName: "execs"

        onExportRequested: function(path, format, opts) {
            AppCore.accountService.exportExecs(path, format, opts, {
                "category": EnumHelpers.marketTypeToApiString(
                   AppCore.marketState.current.info.category),
                "symbol":   AppCore.marketState.current.info.symbol
            })
        }
    }

    WalletPopup {
        id: assetPopup

        onHoveredChanged: function(hovered) {
            if (hovered)
                closeTimer.stop()
            else
                closeTimer.restart()
        }
    }

    Timer {
        id: closeTimer
        interval: 300
        repeat: false
        onTriggered: assetPopup.close()
    }

// ++++++++++++++++++++++++++++++++++++++++++   UI  ++++++++++++++++++++++++++++++++++++++++++

    // Меню
    menuBar: CustomMenuBar {
        menuModel: [
            {
                text: qsTr("Application"),
                items: [
                    {
                        text: qsTr("Settings"),
                        clicked: function() { settingsWindowLoader.active = true }
                    },
                    {
                        text: qsTr("Logs"),
                        clicked: function() {
                            if (logWindowLoader.item)
                                logWindowLoader.item.raise()
                            else logWindowLoader.active = true
                        }
                    },
                    { text: "---" },
                    { text: qsTr("About") },
                    {
                        text: qsTr("Exit"),
                        clicked: function() {
                            Qt.quit()
                        }
                    }
                ]
            },
            {
                text: qsTr("User"),
                clicked: function() { mainStack.showUserScreen() }
            },
            {
                text: qsTr("Trade"),
                clicked: function() {
                    mainStack.showTradeScreen()
                }
            },
            {
                text: qsTr("Market"),
                items: [
                    {
                        text: qsTr("Instruments"),
                        clicked: function() {
                            selecterTradeWindowLoader.active = true
                        }
                    },
                    {
                        text: qsTr("Order"),
                        clicked: function() {
                            orderDialog.open()
                        },
                        enabled: ConfigurationManager.enableTrades && AppCore.accountState.accountVerified
                    }
                ]
            },
            {

                text: qsTr("Export"),
                items: [
                    { text: qsTr("Klines"), clicked: function() { klinesExportDialog.open() } },
                    { text: qsTr("Public trades"), clicked: function() { tradesExportDialog.open() } },
                    { text: "---" },
                    { text: qsTr("Orders"), clicked: function() { ordersExportDialog.open() } },
                    { text: qsTr("Executions"), clicked: function() { execsExportDialog.open() } },
                    { text: qsTr("Positions") }
                ]
            }
        ]
    }

    // Главное окно
    Page {
        id: mainPage
        anchors.fill: parent

        background: Rectangle { color: Theme.windowColor }

        header: CustomToolBar {
            modelToolButtons: [
                { id: "btn_run",        icon: "qrc:/icons/icon_play.png" },
                { id: "btn_restart",    icon: "qrc:/icons/icon_restart.png" },
                { id: "btn_stop",       icon: "qrc:/icons/icon_stop.png" },
                { id: "btn_wallet",     icon: "qrc:/icons/icon_wallet.png", rightAligned: true },
            ]
            onButtonClicked: function(id, name) {
                if (id === "btn_run")
                    AppCore.marketService.run()
                else if (id === "btn_restart")
                    AppCore.marketService.restart()
                else if (id === "btn_stop")
                    AppCore.marketService.shutdown()
            }
            onButtonHovered: function(id, item, hovered) {
                if (id !== "btn_wallet")
                    return

                if (hovered) {
                    closeTimer.stop()
                    if (!assetPopup.visible) {
                        assetPopup.parent = item
                        assetPopup.x = item.width - assetPopup.width
                        assetPopup.y = item.height + 4
                        assetPopup.open()
                    }
                } else {
                    closeTimer.restart()
                }
            }
        }

        StackView {
            id: mainStack
            anchors.fill: parent

            initialItem: TradeScreen {}

            // Метод для вызова экрана торговли
            function showTradeScreen()
            {
                mainStack.replace("Views/TradeScreen.qml", StackView.Immediate)
            }

            // Метод для вызова экрана пользователя
            function showUserScreen()
            {
                mainStack.replace("Views/UserScreen.qml", StackView.Immediate)
            }

        } // mainStack

    } // mainLayout

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

}
