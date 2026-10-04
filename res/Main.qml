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

    /*Connections {
        target: AppCore.marketService

        function onMessageReceived(msg) { addLog("info", "Market", msg ) }

        function onErrorOccurred(error) { mainWindow.addLog("error", "Market", error) }

        function onDownloadProgress(received, total) {
            if (total > 0)
                statusWidget.progressValue = received / total * 100
        }
    }

    Connections {
        target: AppCore.accountService

        function onMessageReceived(msg) { addLog("info", "Account", msg ) }

        function onErrorOccurred(error) { mainWindow.addLog("error", "Account", error) }

        function onDownloadProgress(received, total) {
            if (total > 0)
                statusWidget.progressValue = received / total * 100
        }
    }

    Connections {
        target: ConfigurationManager

        function onErrorOccurred(error) { mainWindow.addLog("error", "Config", error) }
    }*/

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
        source: "Views/SelecterTradeWindow.qml"

        onLoaded: {
            item.tradeSelected.connect(function(symbol) {
                AppCore.marketService.subscribeSymbol(symbol)
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

// ++++++++++++++++++++++++++++++++++++++++++   UI  ++++++++++++++++++++++++++++++++++++++++++

    // Меню
    menuBar: CustomMenuBar {
        menuModel: [
            {
                text: "Services",
                items: [
                    {
                        text: "User",
                        clicked: function() { mainStack.showUserScreen() }
                    },
                    {
                        text: "Trade",
                        clicked: function() {
                            mainStack.showTradeScreen()
                        }
                    },
                    {
                        text: "Available trades",
                        clicked: function() {
                            selecterTradeWindowLoader.active = true
                        }
                    }

                ]
            },
            {
                text: "Repository",
                items: [
                    {
                        text: "Load",
                        items: [
                            {
                                text: "Spot",
                                clicked: function() { AppCore.marketService.loadTradePairs(Tools.MarketType.Spot) }
                            },
                            {
                                text: "Linear",
                                clicked: function() { AppCore.marketService.loadTradePairs(Tools.MarketType.Linear) }
                            },
                            {
                                text: "Inverse",
                                clicked: function() { AppCore.marketService.loadTradePairs(Tools.MarketType.Inverse) }
                            },
                            {
                                text: "Option",
                                clicked: function() { AppCore.marketService.loadTradePairs(Tools.MarketType.Option) }
                            }
                        ]
                    },
                    { text: "---" },
                    {
                        text: "Export",
                        items: [
                            {
                                text: "Klines"
                            }
                        ]
                    },
                ]
            },
            {
                text: "View",
                items: [
                    {
                        text: "Order book",
                        clicked: function() { mainStack.currentItem.showOrderbook() }
                    },
                    {
                        text: "Recent trades",
                        clicked: function() { mainStack.currentItem.showRecentTrades() }
                    },
                    {
                        text: "Volume chart",
                        clicked: function() { mainStack.currentItem.enableVolumeChart = !mainStack.currentItem.enableVolumeChart }
                    }
                ]
            },
            {
                text: "Settings",
                clicked: function() { settingsWindowLoader.active = true }
            },
            {
                text: "Logs",
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
