import QtQuick
import QtQuick.Controls
import Theme 1.0

Menu {
    id: root

    leftPadding: Theme.padding / 2
    rightPadding: Theme.padding / 2
    topPadding: Theme.padding / 2
    bottomPadding: Theme.padding / 2

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: contentHeight + topPadding + bottomPadding

        color: Theme.translucentMenus
               ? Qt.alpha(Theme.windowColor, Theme.menuOpacity)
               : Theme.windowColor
        border.color: Theme.borderColor
        border.width: Theme.borderWidth
        radius: Theme.radius
    }

    // Кастомный делегат
    delegate: CustomMenuItem {}
}
