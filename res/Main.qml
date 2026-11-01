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
    minimumWidth: 800
    minimumHeight: 600
    width: 1000
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
            item.instrumentSelected.connect(function(inst) {
                AppCore.marketService.subscribeInstrument()
                AppCore.accountService.requestOrderHistory({ "category": inst.category })
                AppCore.accountService.requestOpenOrders({ "category": inst.category })
                AppCore.accountService.requestTradeHistory({ "category": inst.category })
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
                "category": EnumHelpers.marketTypeToApiString(AppCore.marketState.currentCategory),
                "symbol":   AppCore.marketState.currentSymbol,
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
                "category": EnumHelpers.marketTypeToApiString(AppCore.marketState.currentCategory),
                "symbol":   AppCore.marketState.currentSymbol
            })
        }
    }

    ExportDialog {
        id: ordersExportDialog
        title: qsTr("Export orders")
        defaultName: "orders"

        onExportRequested: function(path, format, opts) {
            AppCore.accountService.exportOrders(path, format, opts, {
                "category": EnumHelpers.marketTypeToApiString(AppCore.marketState.currentCategory),
                "symbol":   AppCore.marketState.currentSymbol
            })
        }
    }

    ExportDialog {
        id: execsExportDialog
        title: qsTr("Export execs")
        defaultName: "execs"

        onExportRequested: function(path, format, opts) {
            AppCore.accountService.exportExecs(path, format, opts, {
                "category": EnumHelpers.marketTypeToApiString(AppCore.marketState.currentCategory),
                "symbol":   AppCore.marketState.currentSymbol
            })
        }
    }

// ++++++++++++++++++++++++++++++++++++++++++   UI  ++++++++++++++++++++++++++++++++++++++++++

    // Меню
    menuBar: CustomMenuBar {
        menuModel: [
            {
                text: qsTr("Services"),
                items: [
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
                        text: qsTr("Instruments"),
                        clicked: function() {
                            selecterTradeWindowLoader.active = true
                        }
                    },
                    {
                        text: qsTr("Order"),
                        clicked: function() {
                            orderDialog.open()
                        }
                    },
                    { text: "---" },
                    {
                        text: qsTr("Exit"),
                        clicked: function() {
                            Qt.quit()
                        }
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
            },
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
                { id: "btn_run", icon: "qrc:/icons/icon_play.png" },
                { id: "btn_restart", icon: "qrc:/icons/icon_restart.png" },
                { id: "btn_stop", icon: "qrc:/icons/icon_stop.png" }
            ]
            onToolButtonClicked: function(id, name) {
                if (id === "btn_run")
                    AppCore.marketService.run()
                else if (id === "btn_restart")
                    AppCore.marketService.restart()
                else if (id === "btn_stop")
                    AppCore.marketService.shutdown()
            }
        }

        StackView {
            id: mainStack
            anchors.fill: parent

            initialItem: UserScreen {}

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

        /*footer: Rectangle {
            color: Theme.toolBarColor
            height: 30

            RowLayout {
                anchors.fill: parent

                CustomStatusBar {
                    id: statusWidget
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    autoHide: false
                }

                Item { Layout.fillWidth: true }

                PingIndicator {
                    width: 30
                    height: 30
                    thickness: 1
                    pingValue: AppCore.marketState.pingMs
                }
            }
        }*/

    } // mainLayout

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

}
