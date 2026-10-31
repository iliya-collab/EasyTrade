import QtQuick 2.15
import QtQuick.Shapes 1.15
import Components.Configs 1.0

Item {
    id: root
    width: PingConfig.size
    height: PingConfig.size

    property int pingValue: 0
    property int maxPing: PingConfig.defaultMaxPing
    property real thickness: PingConfig.thickness // толщина линии индикатора

    onPingValueChanged: maxPing = Math.max(maxPing, pingValue)

    readonly property real angle: Math.min(pingValue / maxPing, 1.0) * 360
    readonly property color pingColor: PingConfig.colorFor(pingValue)

    // Фоновая серая дуга
    Shape {
        anchors.fill: parent
        ShapePath {
            strokeColor: PingConfig.trackColor
            strokeWidth: root.thickness
            fillColor: "transparent"
            capStyle: ShapePath.RoundCap

            startX: root.width / 2
            startY: root.thickness / 2
            PathArc {
                x: root.width - root.thickness / 2
                y: root.height / 2
                radiusX: root.width / 2 - root.thickness / 2
                radiusY: root.height / 2 - root.thickness / 2
                useLargeArc: false
                direction: PathArc.Clockwise
                xAxisRotation: 0
            }
        }
    }

    // Активная дуга, отражающая пинг
    Shape {
        anchors.fill: parent
        visible: root.pingValue > 0

        ShapePath {
            strokeColor: root.pingColor
            strokeWidth: root.thickness
            fillColor: "transparent"
            capStyle: ShapePath.RoundCap

            // Начало дуги сверху (угол поворота определяется математически)
            startX: root.width / 2
            startY: root.thickness / 2

            PathAngleArc {
                centerX: root.width / 2
                centerY: root.height / 2
                radiusX: root.width / 2 - root.thickness / 2
                radiusY: root.height / 2 - root.thickness / 2
                startAngle: PingConfig.startAngle // Старт с 12 часов
                sweepAngle: root.angle
            }
        }
    }

    // Текстовое отображение значения в центре
    Text {
        anchors.centerIn: parent
        text: root.pingValue > 0 ? root.pingValue : PingConfig.emptyText
        color: root.pingValue > 0 ? root.pingColor : PingConfig.idleTextColor
        font.pixelSize: PingConfig.fontSize
    }
} // root