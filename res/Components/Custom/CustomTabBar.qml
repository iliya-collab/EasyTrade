import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Theme 1.0

TabBar {
    id: root

    property var modelTabs: []

    background: Rectangle {
        color: Theme.toolBarColor
    }

    Repeater {
        model: root.modelTabs

        TabButton {
            id: tabs
            text: modelData.text

            z: tabs.checked ? 1 : 0

            background: Rectangle {
                color: tabs.checked ? Theme.selectColor
                     : tabs.pressed ? Theme.pressColor
                     : tabs.hovered ? Theme.hoverColor
                     : "transparent"
                radius: Theme.radius
                border.color: Theme.borderColor
                border.width: Theme.borderWidth

                transform: Translate {
                    y: tabs.checked ? -4 : 0
                    Behavior on y {
                        NumberAnimation { duration: 120; easing.type: Easing.OutCubic }
                    }
                }

                Behavior on color {
                    ColorAnimation { duration: 150 }
                }
            }
            contentItem: Text {
                text: tabs.text
                font.pixelSize: Theme.fontSizeBody
                font.family: Theme.fontFamily
                color: Theme.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                transform: Translate {
                    y: tabs.checked ? -4 : 0
                    Behavior on y {
                        NumberAnimation { duration: 120; easing.type: Easing.OutCubic }
                    }
                }
            }
        } // ToolButton
    } // Repeater
}
