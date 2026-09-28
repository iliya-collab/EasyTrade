import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls 2.15
import Components.Crypto 1.0
import Components.Custom 1.0
import Application.Core 1.0

Item {
    id: root

    property string currentCategory: "spot"
    property string currentTrade: ""
    property string currentInterval: "1"
    property alias enableVolumeChart: chart.enableShowVolumes

    // Список таймфреймов для панели: подпись -> значение interval для Bybit API
    readonly property var timeframes: [
        { label: "1m",  value: "1" },
        { label: "3m",  value: "3" },
        { label: "5m",  value: "5" },
        { label: "15m", value: "15" },
        { label: "30m", value: "30" },
        { label: "1h",  value: "60" },
        { label: "2h",  value: "120" },
        { label: "4h",  value: "240" },
        { label: "6h",  value: "360" },
        { label: "12h",  value: "720" },
        { label: "D",  value: "D" },
        { label: "W",  value: "W" },
        { label: "M",  value: "M" }
    ]

    onCurrentCategoryChanged: reloadFull()
    onCurrentTradeChanged: reloadFull()
    onCurrentIntervalChanged: ensureHistoryLoaded(currentInterval)

    function reloadFull()
    {
        if (currentTrade === "")
            return

        AppCore.marketState.klineStore.clear()
        ensureHistoryLoaded(currentInterval)
    }

    function ensureHistoryLoaded(interval)
    {
        if (currentTrade === "")
            return

        chart.resetChart()

        AppCore.marketService.loadKlines(
            {
                "category": currentCategory,
                "symbol": currentTrade,
                "interval": interval,
                "limit": 200
            }
        )
    }

    KlineSeriesModel {
        id: seriesModel
        klineStore: AppCore.marketState.klineStore
        timeframe: root.currentInterval
    }


    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            spacing: 8

            // Панель таймфреймов
            CustomComboBox {
                id: timeframeCombo
                Layout.preferredHeight: 30
                Layout.preferredWidth: 90

                model: root.timeframes
                textRole: "label"
                valueRole: "value"

                // Синхронизация ComboBox -> currentInterval (по выбору пользователя)
                onActivated: root.currentInterval = currentValue

                // Синхронизация currentInterval -> ComboBox (если значение меняется извне)
                Component.onCompleted: currentIndex = indexOfValue(root.currentInterval)

                Connections {
                    target: root
                    function onCurrentIntervalChanged() {
                        var idx = timeframeCombo.indexOfValue(root.currentInterval)
                        if (idx !== -1 && idx !== timeframeCombo.currentIndex)
                        {
                            timeframeCombo.currentIndex = idx
                            AppCore.marketState.klineSeries.timeframe = timeframeCombo.currentValue
                        }
                    }
                }
            }

            // Панель зума
            Rectangle {
               color: "#2a2a2a"
               Layout.preferredHeight: 30
               Layout.preferredWidth: 300

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 4
                    spacing: 4

                    CustomLabel { text: "Zoom:" }

                    CustomButton {
                        text: "-"
                        Layout.preferredWidth: 50
                        Layout.fillHeight: true
                        onClicked: chart.zoomOut()
                    }

                    CustomButton {
                        text: "+"
                        Layout.preferredWidth: 50
                        Layout.fillHeight: true
                        onClicked: chart.zoomIn()
                    }

                    CustomButton {
                        text: "⟲"
                        Layout.preferredWidth: 50
                        Layout.fillHeight: true
                        onClicked: chart.resetZoom()
                    }
                }
            }

        } // RowLayout

        KlineChart {
            id: chart
            Layout.fillHeight: true
            Layout.fillWidth: true
            klineSeries: seriesModel
        }

    } // ColumnLayout
} // root

