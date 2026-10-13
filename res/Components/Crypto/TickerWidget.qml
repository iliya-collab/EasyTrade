import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import Application.Core 1.0
import Theme 1.0

Rectangle {
    id: root
    color: "transparent"
    implicitWidth: 700
    implicitHeight: 180

    readonly property Ticker tckr : AppCore.marketState.ticker

    // === Данные ===
    property string symbol:     tckr.symbol ? tckr.symbol : "Unknown"
    property real low:          tckr.low24h ? tckr.low24h : 0
    property real high:         tckr.high24h ? tckr.high24h : 1000
    property real current:      tckr.lastPrice ? tckr.lastPrice : high * 0.25

    readonly property real currentFrac: high > low
        ? Math.max(0, Math.min(1, (current - low) / (high - low)))
        : 0.5

    // === Направление движения ===
    //  1 — цена движется вправо (к 24h High)
    // -1 — цена движется влево (к 24h Low)
    //  0 — направление ещё неизвестно
    property real prevCurrent: 0
    property int  direction: 0

    onCurrentChanged: {
        if (prevCurrent !== 0 && current !== prevCurrent)
            direction = current > prevCurrent ? 1 : -1
        prevCurrent = current
    }

    // При смене пары старое направление не имеет смысла
    onSymbolChanged: {
        prevCurrent = 0
        direction = 0
    }

    // === Цвета ===
    readonly property color bgColor:      "#1e222b"
    readonly property color trackColor:   "#5b8def"
    readonly property color lowColor:     Theme.downColor
    readonly property color highColor:    Theme.upColor
    readonly property color textColor:    "#c8ccd6"
    readonly property color titleColor:   "#f5f6fa"
    readonly property color axisColor:    "#8b91a1"

    // Цвет текущей цены: зелёный — к High, красный — к Low
    readonly property color directionColor:
        direction > 0 ? Theme.upColor : (direction < 0 ? Theme.downColor : Theme.currentColor)

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
            // Low всегда на левом краю шкалы, High — на правом
            readonly property real lowX:     padL
            readonly property real highX:    padL + trackW
            readonly property real currentX: padL + trackW * root.currentFrac

            // --- Стрелка направления рядом с точкой Current ---
            readonly property real arrowBaseX: currentX + root.direction * 12
            readonly property real arrowTipX:  arrowBaseX + root.direction * 8

            // Единый Shape: трек + маркеры + вертикаль Current + стрелка
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
                    strokeColor: root.directionColor
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

                // --- Current: точка ---
                ShapePath {
                    strokeWidth: 2
                    strokeColor: root.bgColor
                    fillColor: root.directionColor

                    PathAngleArc {
                        centerX: scaleArea.currentX
                        centerY: scaleArea.trackCenterY
                        radiusX: 7
                        radiusY: 7
                        startAngle: 0
                        sweepAngle: 360
                    }
                }

                // --- Current: стрелка направления (вправо — к High, влево — к Low) ---
                ShapePath {
                    strokeWidth: 0
                    fillColor: root.direction === 0 ? "transparent" : root.directionColor

                    startX: scaleArea.arrowBaseX
                    startY: scaleArea.trackCenterY - 5

                    PathLine {
                        x: scaleArea.arrowTipX
                        y: scaleArea.trackCenterY
                    }
                    PathLine {
                        x: scaleArea.arrowBaseX
                        y: scaleArea.trackCenterY + 5
                    }
                    PathLine {
                        x: scaleArea.arrowBaseX
                        y: scaleArea.trackCenterY - 5
                    }
                }
            }

            // Подписи над треком
            Text {
                x: scaleArea.lowX
                y: 0
                text: "24h Low: " + root.low
                color: root.lowColor
                font.pixelSize: 12
            }
            Text {
                x: scaleArea.currentX - width / 2
                y: 0
                text: "Current: " + root.current
                color: root.directionColor
                font.pixelSize: 12
                font.bold: true
            }
            Text {
                x: scaleArea.highX - width
                y: 0
                text: "24h High: " + root.high
                color: root.highColor
                font.pixelSize: 12
            }
        }

        // Ось X
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
