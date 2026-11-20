import QtQuick
import QtQuick.Controls
import Theme 1.0

MenuItem {
    id: root

    // Отступы для содержимого
    readonly property int indicatorWidth: Theme.menuIndicatorWidth
    readonly property bool translucent: Theme.translucentMenus

    leftPadding: Theme.padding + (checkable ? indicatorWidth + Theme.padding : 0)
    rightPadding: Theme.padding
    topPadding: Theme.padding / 2
    bottomPadding: Theme.padding / 2

    font.pixelSize: Theme.fontSizeBody
    font.family: Theme.fontFamily

    // Фон пункта
    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 32

        readonly property real a: root.translucent ? Theme.menuOpacity : 1.0

        color: !root.enabled     ? Qt.alpha(Theme.disabledColor, a)
             : root.down         ? Qt.alpha(Theme.pressColor,    a)
             : root.highlighted  ? Qt.alpha(Theme.hoverColor,    a)
             : "transparent"

        Behavior on color { ColorAnimation { duration: 100 } }
    }

    // Содержимое (текст + иконка)
    contentItem: Row {
        spacing: Theme.padding

        // Иконка (если задана)
        Image {
            id: iconImage
            visible: root.icon.source.toString() !== ""
            source: root.icon.source
            sourceSize.width: Theme.fontSizeBody
            sourceSize.height: Theme.fontSizeBody
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: label
            text: root.text
            font: root.font
            color: !root.enabled ? Theme.disabledTextColor : Theme.textColor
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight

            Behavior on color { ColorAnimation { duration: 100 } }
        }
    }

    // Индикатор (галочка для checkable)
    indicator: Rectangle {
        implicitWidth: root.indicatorWidth
        implicitHeight: root.indicatorWidth

        x: Theme.padding
        y: root.topPadding + (root.availableHeight - height) / 2

        radius: 2
        border.color: root.enabled ? Theme.borderColor : Theme.disabledBorderColor
        border.width: Theme.borderWidth
        color: root.checked ? Theme.pressColor : "transparent"

        visible: root.checkable

        Text {
            anchors.centerIn: parent
            text: "✓"
            font.pixelSize: 12
            color: Theme.textColor
            visible: root.checked
        }
    }
}