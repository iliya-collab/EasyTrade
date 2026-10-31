import QtQuick
import QtQuick.Controls
import Theme 1.0

Button {
    id: root

    implicitWidth: contentItem.implicitWidth + leftPadding + rightPadding
    implicitHeight: contentItem.implicitHeight + topPadding + bottomPadding

    leftPadding: Theme.padding
    rightPadding: Theme.padding
    topPadding: Theme.padding / 2
    bottomPadding: Theme.padding / 2

    font.pixelSize: Theme.fontSizeBody
    font.family: Theme.fontFamily

    background: Rectangle {
        color: !root.enabled ? Theme.disabledButtonColor
             : root.pressed  ? Theme.pressColor
             : root.hovered  ? Theme.hoverColor
             : Theme.buttonColor

        border.color: root.enabled ? Theme.borderColor : Theme.disabledBorderColor
        border.width: Theme.borderWidth
        radius: Theme.radius

        Behavior on color { ColorAnimation { duration: 100 } }
    }

    contentItem: Text {
        text: root.text

        font: root.font

        color: root.enabled ? Theme.textColor : Theme.disabledTextColor
        Behavior on color { ColorAnimation { duration: 100 } }

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}