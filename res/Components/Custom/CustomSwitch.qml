import QtQuick
import QtQuick.Controls

import Theme 1.0

Switch {
    id: root

    implicitWidth: contentItem.implicitWidth + leftPadding + rightPadding
    implicitHeight: Math.max(contentItem.implicitHeight + topPadding + bottomPadding,
                             indicator.implicitHeight + topPadding + bottomPadding)

    spacing: Theme.spacing
    padding: Theme.padding
    topPadding: Theme.padding / 2
    bottomPadding: Theme.padding / 2

    indicator: Item {
        id: indicatorRoot
        implicitWidth: Theme.switchWidth
        implicitHeight: Theme.switchHeight

        x: root.leftPadding
        y: (root.height - height) / 2

        // Фон переключателя (дорожка)
        Rectangle {
            id: track
            anchors.fill: parent
            radius: height / 2
            color: !root.enabled ? Theme.disabledButtonColor
                 : root.checked   ? Theme.switchCheckedColor
                 : Theme.switchBackgroundColor
            border.width: Theme.borderWidth
            border.color: !root.enabled ? Theme.disabledBorderColor
                        : root.hovered   ? Theme.hoverTextFieldColor
                        : Theme.borderColor

            Behavior on color {
                ColorAnimation { duration: 120 }
            }
        }

        // Ползунок
        Rectangle {
            id: handle
            width: parent.height - 4
            height: parent.height - 4
            radius: height / 2
            y: 2
            x: root.checked
               ? parent.width - width - 2
               : 2

            color: !root.enabled ? Theme.disabledTextColor
                 : root.pressed  ? Theme.pressColor
                 : Theme.switchHandleColor

            Behavior on x {
                NumberAnimation { duration: 120; easing.type: Easing.InOutQuad }
            }
        }
    }

    contentItem: Text {
        text: root.text
        font.family: Theme.fontFamily
        font.pixelSize: Theme.fontSizeBody
        color: root.enabled ? Theme.textColor : Theme.disabledTextColor
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        leftPadding: root.indicator.width + root.spacing
    }
}