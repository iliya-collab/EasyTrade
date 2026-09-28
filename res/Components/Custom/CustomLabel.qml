import QtQuick
import QtQuick.Controls
import Theme 1.0

Label {
    id: root

    font.family: Theme.fontFamily
    font.pixelSize: Theme.fontSizeBody
    color: root.enabled ? Theme.textColor : Theme.disabledTextColor
    verticalAlignment: Text.AlignVCenter
    elide: Text.ElideRight
}