import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import Application.Core 1.0

Rectangle {
    id: root
    color: "transparent"
    implicitWidth: 700
    implicitHeight: 180

    readonly property Ticker tckr : AppCore.marketState.ticker

    // === Данные ===
    property string symbol : tckr.symbol ? tckr.symbol : "Unknown"
    property real low: tckr.low24h ? tckr.low24h : 0
    property real high: tckr.high24h ? tckr.high24h : 1000
    property real current: tckr.lastPrice ? tckr.lastPrice : high * 0.25

    readonly property real currentFrac:
        Math.max(0, Math.min(1, (current - low) / (high - low)))

    // === Цвета ===
    readonly property color bgColor:      "#1e222b"
    readonly property color trackColor:   "#5b8def"
    readonly property color lowColor:     "#f0a04b"
    readonly property color highColor:    "#5b8def"
    readonly property color currentColor: "#e83e8c"
    readonly property color textColor:    "#c8ccd6"
    readonly property color titleColor:   "#f5f6fa"
    readonly property color axisColor:    "#8b91a1"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 12

        // Заголовок
        Text {
            text: symbol
            color: root.titleColor
            font.pixelSize: 18
            font.bold: true
        }

        Item { Layout.preferredHeight: 8 }

        // Зона шкалы
        Item {
            id: scaleArea
            Layout.fillWidth: true
            Layout.preferredHeight: 70

            // --- Геометрия трека ---
            readonly property real trackY0: 34              // верх трека
            readonly property real trackH:  14              // высота трека
            readonly property real trackR:  trackH / 2      // радиус скругления
            readonly property real padL:    40
            readonly property real padR:    40
            readonly property real trackW:  width - padL - padR
            readonly property real trackX0: padL            // левый край трека

            // алиасы для читаемости
            readonly property real trackY:       trackY0
            readonly property real trackCenterY: trackY0 + trackH / 2

            // --- Координаты маркеров ---
            readonly property real lowX:
                padL + trackW * root.low / (root.high - root.low)
            readonly property real highX:
                padL + trackW * root.high / (root.high - root.low)
            readonly property real currentX:
                padL + trackW * root.currentFrac

            // Единый Shape: трек + маркеры + вертикаль Current
            Shape {
                anchors.fill: parent
                layer.enabled: true
                layer.samples: 4

                // --- Трек ---
                ShapePath {
                    strokeWidth: 0
                    fillColor: root.trackColor

                    startX: scaleArea.trackX0 + scaleArea.trackR
                    startY: scaleArea.trackY0

                    // верх
                    PathLine {
                        x: scaleArea.trackX0 + scaleArea.trackW - scaleArea.trackR
                        y: scaleArea.trackY0
                    }
                    PathArc {
                        x: scaleArea.trackX0 + scaleArea.trackW
                        y: scaleArea.trackY0 + scaleArea.trackR
                        radiusX: scaleArea.trackR
                        radiusY: scaleArea.trackR
                    }

                    // право
                    PathLine {
                        x: scaleArea.trackX0 + scaleArea.trackW
                        y: scaleArea.trackY0 + scaleArea.trackH - scaleArea.trackR
                    }
                    PathArc {
                        x: scaleArea.trackX0 + scaleArea.trackW - scaleArea.trackR
                        y: scaleArea.trackY0 + scaleArea.trackH
                        radiusX: scaleArea.trackR
                        radiusY: scaleArea.trackR
                    }

                    // низ
                    PathLine {
                        x: scaleArea.trackX0 + scaleArea.trackR
                        y: scaleArea.trackY0 + scaleArea.trackH
                    }
                    PathArc {
                        x: scaleArea.trackX0
                        y: scaleArea.trackY0 + scaleArea.trackH - scaleArea.trackR
                        radiusX: scaleArea.trackR
                        radiusY: scaleArea.trackR
                    }

                    // лево
                    PathLine {
                        x: scaleArea.trackX0
                        y: scaleArea.trackY0 + scaleArea.trackR
                    }
                    PathArc {
                        x: scaleArea.trackX0 + scaleArea.trackR
                        y: scaleArea.trackY0
                        radiusX: scaleArea.trackR
                        radiusY: scaleArea.trackR
                    }
                }

                // --- Маркер Low ---
                ShapePath {
                    strokeWidth: 2
                    strokeColor: root.bgColor
                    fillColor: root.lowColor

                    PathAngleArc {
                        centerX: scaleArea.lowX
                        centerY: scaleArea.trackCenterY
                        radiusX: 6
                        radiusY: 6
                        startAngle: 0
                        sweepAngle: 360
                    }
                }

                // --- Маркер High ---
                ShapePath {
                    strokeWidth: 2
                    strokeColor: root.bgColor
                    fillColor: root.highColor

                    PathAngleArc {
                        centerX: scaleArea.highX
                        centerY: scaleArea.trackCenterY
                        radiusX: 6
                        radiusY: 6
                        startAngle: 0
                        sweepAngle: 360
                    }
                }

                // --- Current: вертикальная линия ---
                ShapePath {
                    strokeWidth: 2
                    strokeColor: root.lowColor
                    fillColor: "transparent"
                    capStyle: ShapePath.FlatCap

                    PathMove {
                        x: scaleArea.currentX
                        y: scaleArea.trackY0 - 14
                    }
                    PathLine {
                        x: scaleArea.currentX
                        y: scaleArea.trackY0 + scaleArea.trackH + 14
                    }
                }

                // --- Current: розовая точка ---
                ShapePath {
                    strokeWidth: 2
                    strokeColor: root.bgColor
                    fillColor: root.currentColor

                    PathAngleArc {
                        centerX: scaleArea.currentX
                        centerY: scaleArea.trackCenterY
                        radiusX: 7
                        radiusY: 7
                        startAngle: 0
                        sweepAngle: 360
                    }
                }
            }

            // 3.2 Подписи над треком
            Text {
                x: scaleArea.lowX
                y: 0
                text: "24h Low: " + root.low
                color: root.textColor
                font.pixelSize: 12
            }
            Text {
                x: scaleArea.currentX - width / 2
                y: 0
                text: "Current: " + root.current
                color: root.titleColor
                font.pixelSize: 12
                font.bold: true
            }
            Text {
                x: scaleArea.highX - width
                y: 0
                text: "24h High: " + root.high
                color: root.textColor
                font.pixelSize: 12
            }
        }

        // 4. Ось X
        Item {
            id: axis
            Layout.fillWidth: true
            Layout.preferredHeight: 36

            readonly property real padL: 40
            readonly property real padR: 40
            readonly property int  ticks: 10

            Repeater {
                model: axis.ticks + 1
                delegate: Text {
                    property real value: root.low
                        + (root.high - root.low) * index / axis.ticks

                    x: axis.padL
                       + (axis.width - axis.padL - axis.padR) * index / axis.ticks
                       - width / 2
                    y: 0
                    text: (value / 1000).toFixed(1) + "k"
                    color: root.axisColor
                    font.pixelSize: 11
                }
            }
        }
    }
}