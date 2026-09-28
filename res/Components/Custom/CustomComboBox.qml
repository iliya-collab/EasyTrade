import QtQuick
import QtQuick.Controls

import Theme 1.0

ComboBox {
    id: root

    implicitWidth: Math.max(180,
                            contentItem.implicitWidth
                            + leftPadding + rightPadding
                            + indicator.width + spacing)
    implicitHeight: Math.max(contentItem.implicitHeight + topPadding + bottomPadding,
                             Theme.fontSizeBody * 2.2)

    padding: Theme.padding
    topPadding: Theme.padding / 2
    bottomPadding: Theme.padding / 2
    spacing: Theme.spacing

    font.family: Theme.fontFamily
    font.pixelSize: Theme.fontSizeBody

    // ---------- Фон ----------
    background: Rectangle {
        color: !root.enabled ? Theme.disabledButtonColor
             : root.pressed  ? Theme.pressColor
             : root.hovered  ? Theme.hoverColor
             : Theme.textFieldColor
        border.width: Theme.borderWidth
        border.color: !root.enabled ? Theme.disabledBorderColor
                    : root.activeFocus ? Theme.indicatorColor
                    : root.hovered     ? Theme.hoverTextFieldColor
                    : Theme.borderColor
        radius: Theme.radius
    }

    // ---------- Текст выбранного элемента ----------
    contentItem: Text {
        leftPadding: root.leftPadding
        rightPadding: root.indicator.width + root.spacing
        topPadding: root.topPadding
        bottomPadding: root.bottomPadding

        text: root.displayText
        font: root.font
        color: root.enabled ? Theme.textColor : Theme.disabledTextColor
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    // ---------- Стрелка ----------
    indicator: Text {
        x: root.width - width - root.rightPadding
        y: (root.height - height) / 2

        text: "▾"
        font.family: Theme.fontFamily
        font.pixelSize: Theme.fontSizeBody * 1.2
        color: root.enabled ? Theme.textColor : Theme.disabledTextColor

        rotation: root.popup.visible ? 180 : 0
        Behavior on rotation {
            NumberAnimation { duration: 120; easing.type: Easing.InOutQuad }
        }
    }

    // ---------- Выпадающий список ----------
    popup: Popup {
        id: popupRoot
        y: root.height + 2
        width: root.width
        implicitHeight: Math.min(root.count * (Theme.fontSizeBody * 2) + topPadding + bottomPadding, 240)

        padding: 1
        topPadding: 4
        bottomPadding: 4

        background: Rectangle {
            color: Theme.textFieldColor
            border.color: Theme.borderColor
            border.width: Theme.borderWidth
            radius: Theme.radius
        }

        contentItem: ListView {
            id: listView
            clip: true
            model: root.model
            currentIndex: root.highlightedIndex

            delegate: ItemDelegate {
                id: itemRoot
                width: popupRoot.width
                implicitHeight: Theme.fontSizeBody * 2

                onClicked: {
                    root.currentIndex = index
                    root.popup.close()
                    root.activated(index)
                }

                contentItem: Text {
                    text: root.textRole && root.textRole.length > 0
                          ? model[root.textRole]
                          : modelData

                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeBody
                    color: itemRoot.highlighted || itemRoot.pressed
                           ? Theme.windowColor
                           : (root.enabled ? Theme.textColor : Theme.disabledTextColor)
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    leftPadding: Theme.padding
                    rightPadding: Theme.padding
                }

                background: Rectangle {
                    color: itemRoot.highlighted ? Theme.hoverColor
                         : itemRoot.pressed     ? Theme.pressColor
                         : "transparent"
                    radius: Theme.radius / 2
                }
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }
}