import QtQuick
import QtQuick.Controls
import Theme 1.0

Button {
    id: root

    implicitWidth: contentItem.implicitWidth + leftPadding + rightPadding
    implicitHeight: contentItem.implicitHeight + topPadding + bottomPadding

    padding: Theme.padding
    topPadding: Theme.padding / 2
    bottomPadding: Theme.padding / 2

    background: Rectangle {
        color: !root.enabled ? Theme.disabledButtonColor
             : root.pressed  ? Theme.pressColor
             : root.hovered  ? Theme.hoverColor
             : Theme.buttonColor
        border.color: root.enabled ? Theme.borderColor : Theme.disabledBorderColor
        border.width: Theme.borderWidth
        radius: Theme.radius
    }

    contentItem: Text {
        text: root.text
        font.family: Theme.fontFamily
        font.pixelSize: Theme.fontSizeBody
        color: root.enabled ? Theme.textColor : Theme.disabledTextColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}