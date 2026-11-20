import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtCore
import Application.Core 1.0
import Components.Configs 1.0
import Components.Custom 1.0
import Theme 1.0

Rectangle {
    id: root
    color: "transparent"
    clip: true

    // === Скрытие ===
    // В свёрнутом состоянии виджет сжимается до тонкой полоски (символ + цена),
    // клик по ней или пункт контекстного меню возвращает шкалу.
    readonly property int collapsedHeight: 24
    property alias hidden: store.hidden

    Settings {
        id: store
        category: "TickerWidget"
        property bool hidden: false
    }

    implicitWidth: TickerConfig.implicitWidth
    implicitHeight: hidden ? collapsedHeight : TickerConfig.implicitHeight

    readonly property tTickerInfo tckr : AppCore.marketState.current.ticker

    // === Данные ===
    property string symbol:     tckr.symbol ? tckr.symbol : TickerConfig.fallbackSymbol
    property real low:          tckr.low24h ? tckr.low24h : 0
    property real high:         tckr.high24h ? tckr.high24h : TickerConfig.fallbackHigh
    property real current:      tckr.lastPrice ? tckr.lastPrice : high * TickerConfig.fallbackCurrentRatio

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
    readonly property color bgColor:      TickerConfig.markerBorderColor
    readonly property color trackColor:   TickerConfig.trackColor
    readonly property color lowColor:     TickerConfig.lowColor
    readonly property color highColor:    TickerConfig.highColor
    readonly property color textColor:    TickerConfig.textColor
    readonly property color titleColor:   TickerConfig.titleColor
    readonly property color axisColor:    TickerConfig.axisColor

    // Цвет текущей цены: зелёный — к High, красный — к Low
    readonly property color directionColor:
        direction > 0 ? highColor : (direction < 0 ? lowColor : TickerConfig.neutralColor)

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: TickerConfig.margins
        spacing: TickerConfig.spacing
        visible: !root.hidden

        // Заголовок
        Text {
            text: symbol
            color: root.titleColor
            font.pixelSize: TickerConfig.titleFontSize
            font.bold: true
        }

        Item { Layout.preferredHeight: TickerConfig.titleSpacerHeight }

        // Зона шкалы
        Item {
            id: scaleArea
            Layout.fillWidth: true
            Layout.preferredHeight: TickerConfig.scaleHeight

            // --- Геометрия трека ---
            readonly property real trackY0: TickerConfig.trackTop              // верх трека
            readonly property real trackH:  TickerConfig.trackHeight              // высота трека
            readonly property real trackR:  trackH / 2      // радиус скругления
            readonly property real padL:    TickerConfig.padLeft
            readonly property real padR:    TickerConfig.padRight
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
            readonly property real arrowBaseX: currentX + root.direction * TickerConfig.arrowOffset
            readonly property real arrowTipX:  arrowBaseX + root.direction * TickerConfig.arrowLength

            // Единый Shape: трек + маркеры + вертикаль Current + стрелка
            Shape {
                anchors.fill: parent
                layer.enabled: true
                layer.samples: TickerConfig.layerSamples

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
                    strokeWidth: TickerConfig.markerBorderWidth
                    strokeColor: root.bgColor
                    fillColor: root.lowColor

                    PathAngleArc {
                        centerX: scaleArea.lowX
                        centerY: scaleArea.trackCenterY
                        radiusX: TickerConfig.markerRadius
                        radiusY: TickerConfig.markerRadius
                        startAngle: 0
                        sweepAngle: 360
                    }
                }

                // --- Маркер High ---
                ShapePath {
                    strokeWidth: TickerConfig.markerBorderWidth
                    strokeColor: root.bgColor
                    fillColor: root.highColor

                    PathAngleArc {
                        centerX: scaleArea.highX
                        centerY: scaleArea.trackCenterY
                        radiusX: TickerConfig.markerRadius
                        radiusY: TickerConfig.markerRadius
                        startAngle: 0
                        sweepAngle: 360
                    }
                }

                // --- Current: вертикальная линия ---
                ShapePath {
                    strokeWidth: TickerConfig.currentLineWidth
                    strokeColor: root.directionColor
                    fillColor: "transparent"
                    capStyle: ShapePath.FlatCap

                    PathMove {
                        x: scaleArea.currentX
                        y: scaleArea.trackY0 - TickerConfig.currentLineExtent
                    }
                    PathLine {
                        x: scaleArea.currentX
                        y: scaleArea.trackY0 + scaleArea.trackH + TickerConfig.currentLineExtent
                    }
                }

                // --- Current: точка ---
                ShapePath {
                    strokeWidth: TickerConfig.markerBorderWidth
                    strokeColor: root.bgColor
                    fillColor: root.directionColor

                    PathAngleArc {
                        centerX: scaleArea.currentX
                        centerY: scaleArea.trackCenterY
                        radiusX: TickerConfig.currentRadius
                        radiusY: TickerConfig.currentRadius
                        startAngle: 0
                        sweepAngle: 360
                    }
                }

                // --- Current: стрелка направления (вправо — к High, влево — к Low) ---
                ShapePath {
                    strokeWidth: 0
                    fillColor: root.direction === 0 ? "transparent" : root.directionColor

                    startX: scaleArea.arrowBaseX
                    startY: scaleArea.trackCenterY - TickerConfig.arrowHalfHeight

                    PathLine {
                        x: scaleArea.arrowTipX
                        y: scaleArea.trackCenterY
                    }
                    PathLine {
                        x: scaleArea.arrowBaseX
                        y: scaleArea.trackCenterY + TickerConfig.arrowHalfHeight
                    }
                    PathLine {
                        x: scaleArea.arrowBaseX
                        y: scaleArea.trackCenterY - TickerConfig.arrowHalfHeight
                    }
                }
            }

            // Подписи над треком
            Text {
                x: scaleArea.lowX
                y: 0
                text: TickerConfig.lowLabel + TickerConfig.formatPrice(root.low)
                color: root.lowColor
                font.pixelSize: TickerConfig.labelFontSize
            }
            Text {
                x: scaleArea.currentX - width / 2
                y: 0
                text: TickerConfig.currentLabel + TickerConfig.formatPrice(root.current)
                color: root.directionColor
                font.pixelSize: TickerConfig.labelFontSize
                font.bold: true
            }
            Text {
                x: scaleArea.highX - width
                y: 0
                text: TickerConfig.highLabel + TickerConfig.formatPrice(root.high)
                color: root.highColor
                font.pixelSize: TickerConfig.labelFontSize
            }
        }

        // Ось X
        Item {
            id: axis
            Layout.fillWidth: true
            Layout.preferredHeight: TickerConfig.axisHeight

            readonly property real padL: TickerConfig.axisPadLeft
            readonly property real padR: TickerConfig.axisPadRight
            readonly property int  ticks: TickerConfig.axisTicks

            Repeater {
                model: axis.ticks + 1
                delegate: Text {
                    property real value: root.low
                        + (root.high - root.low) * index / axis.ticks

                    x: axis.padL
                       + (axis.width - axis.padL - axis.padR) * index / axis.ticks
                       - width / 2
                    y: 0
                    text: TickerConfig.formatAxis(value)
                    color: root.axisColor
                    font.pixelSize: TickerConfig.axisFontSize
                }
            }
        }
    }

    // === Свёрнутая полоска ===
    Item {
        id: collapsedBar
        anchors.fill: parent
        visible: root.hidden

        Row {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: TickerConfig.margins
            spacing: 10

            Text {
                text: "\u25B8"
                color: root.axisColor
                font.pixelSize: TickerConfig.labelFontSize
            }
            Text {
                text: root.symbol
                color: root.titleColor
                font.pixelSize: TickerConfig.labelFontSize
                font.bold: true
            }
            Text {
                text: TickerConfig.formatPrice(root.current)
                color: root.directionColor
                font.pixelSize: TickerConfig.labelFontSize
            }
        }

        TapHandler {
            acceptedButtons: Qt.LeftButton
            onTapped: root.hidden = false
        }
    }

    // === Контекстное меню ===
    TapHandler {
        acceptedButtons: Qt.RightButton
        onTapped: tickerMenu.popup(point.position.x, point.position.y)
    }

    CustomMenu {
        id: tickerMenu

        CustomMenuItem {
            text: root.hidden ? qsTr("Show") : qsTr("Hide")
            onTriggered: root.hidden = !root.hidden
        }
    }
}
