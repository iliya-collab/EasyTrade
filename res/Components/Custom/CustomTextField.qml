import QtQuick
import QtQuick.Controls
import Theme 1.0

TextField {
    id: root

    font.family: Theme.fontFamily
    font.pixelSize: Theme.fontSizeBody
    color: root.enabled ? Theme.textColor : Theme.disabledTextColor
    placeholderTextColor: Theme.placeholderColor
    selectionColor: Theme.selectionColor
    selectedTextColor: Theme.selectedTextColor
    padding: Theme.padding
    verticalAlignment: Text.AlignVCenter

    background: Rectangle {
        color: root.enabled ? Theme.textFieldColor : Theme.disabledFieldColor
        border.color: root.focus ? Theme.focusColor
                    : root.hovered ? Theme.hoverTextFieldColor
                    : Theme.borderColor
        border.width: Theme.borderWidth
        radius: Theme.radius
    }
}
