import QtQuick
import QtQuick.Controls
import Theme 1.0

CheckBox {
    id: root

    implicitWidth: contentItem.implicitWidth + leftPadding + rightPadding
    implicitHeight: Math.max(contentItem.implicitHeight + topPadding + bottomPadding,
                             indicator.implicitHeight + topPadding + bottomPadding)

    spacing: Theme.spacing
    padding: Theme.padding
    topPadding: Theme.padding / 2
    bottomPadding: Theme.padding / 2

    indicator: Rectangle {
        implicitWidth: Theme.widthBox
        implicitHeight: Theme.widthBox
        x: root.leftPadding
        y: (root.height - height) / 2

        color: "transparent"
        border.width: Theme.borderWidth
        border.color: !root.enabled ? Theme.disabledBorderColor
                    : root.checked   ? Theme.indicatorColor
                    : root.hovered   ? Theme.hoverTextFieldColor
                    : Theme.borderColor

        Text {
            anchors.centerIn: parent
            text: "✓"
            font.pixelSize: Theme.widthBox * 0.8
            color: root.enabled ? Theme.indicatorColor : Theme.disabledTextColor
            visible: root.checked
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