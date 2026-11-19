import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Theme 1.0

ToolBar {
    id: root

    property var modelToolButtons: []
    signal buttonClicked(string id, string button)
    signal buttonItemClicked(string id, Item item)
    signal buttonHovered(string id, Item item, bool hovered)

    background: Rectangle {
        color: Theme.toolBarColor
    }

    // === Левая группа ===
    RowLayout {
        anchors.fill: parent
        spacing: Theme.spacing

        Repeater {
            model: root.modelToolButtons.filter(b => !b.rightAligned)

            ToolButton {
                id: btnMenuLeft
                text: modelData.text
                background: Rectangle {
                    color: btnMenuLeft.pressed ?
                               Theme.pressColor :
                               (btnMenuLeft.hovered ?
                                    Theme.hoverColor :
                                    "transparent")
                }
                contentItem: RowLayout {
                    anchors.centerIn: parent

                    Image {
                        visible: !!modelData.icon
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        source: modelData.icon
                        sourceSize: Qt.size(24, 24)
                        fillMode: Image.PreserveAspectFit
                    }

                    CustomText {
                        visible: btnMenuLeft.text !== ""
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        text: btnMenuLeft.text
                        font.pixelSize: Theme.fontSizeSmall
                    }
                }

                onClicked: root.buttonClicked(modelData.id, btnMenuLeft.text)
                onPressed: root.buttonItemClicked(modelData.id, btnMenuLeft)
                onHoveredChanged: root.buttonHovered(modelData.id, btnMenuLeft, hovered)

            } // ToolButton
        } // Repeater

        Item { Layout.fillWidth: true }

        // === Правая группа ===
        Repeater {
            model: root.modelToolButtons.filter(b => b.rightAligned)

            ToolButton {
                id: btnMenuRight
                text: modelData.text
                background: Rectangle {
                    color: btnMenuRight.pressed ?
                               Theme.pressColor :
                               (btnMenuRight.hovered ?
                                    Theme.hoverColor :
                                    "transparent")
                }
                contentItem: RowLayout {
                    anchors.centerIn: parent

                    Image {
                        visible: !!modelData.icon
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        source: modelData.icon
                        sourceSize: Qt.size(24, 24)
                        fillMode: Image.PreserveAspectFit
                    }

                    CustomText {
                        visible: btnMenuRight.text !== ""
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        text: btnMenuRight.text
                        font.pixelSize: Theme.fontSizeSmall
                    }
                }

                onClicked: root.buttonClicked(modelData.id, btnMenuRight.text)
                onPressed: root.buttonItemClicked(modelData.id, btnMenuRight)
                onHoveredChanged: root.buttonHovered(modelData.id, btnMenuRight, hovered)
            } // ToolButton
        } // Repeater

    } // RowLayout
} // CustomToolBar
