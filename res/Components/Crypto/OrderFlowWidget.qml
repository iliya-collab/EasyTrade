import QtQuick
import QtQuick.Controls
import QtQml
import QtCore
import Theme 1.0
import Components.Custom 1.0
import Components.Crypto 1.0

Rectangle {
    id: root
    color: Theme.windowColor
    clip: true

    // Ширина узкой полоски в свёрнутом состоянии
    readonly property int collapsedWidth: 28

    readonly property var views: [
        { title: qsTr("Order book"),    component: orderbookComponent },
        { title: qsTr("Public trades"), component: tradesComponent    }
    ]

    property alias currentIndex: store.viewIndex
    readonly property int safeIndex: Math.max(0, Math.min(currentIndex, views.length - 1))

    property alias hidden: store.hidden

    Settings {
        id: store
        category: "OrderFlowWidget"
        property int  viewIndex: 0
        property bool hidden:    false
    }

    Loader {
        id: contentLoader
        anchors.fill: parent
        anchors.margins: Theme.margins
        active: !root.hidden
        sourceComponent: root.hidden ? null : root.views[root.safeIndex].component
    }

    Component { id: orderbookComponent; OrderbookWidget {} }
    Component { id: tradesComponent;    PublicTradesWidget {} }

    // Полоска в свёрнутом состоянии
    ToolButton {
        anchors.fill: parent
        visible: root.hidden
        text: "‹"
        onClicked: root.hidden = false
    }

    TapHandler {
        acceptedButtons: Qt.RightButton
        onTapped: viewMenu.popup(point.position.x, point.position.y)
    }

    CustomMenu {
        id: viewMenu

        Instantiator {
            model: root.views
            delegate: CustomMenuItem {
                required property int index
                required property var modelData

                text: modelData.title
                checkable: true
                autoExclusive: true
                checked: !root.hidden && root.safeIndex === index
                onTriggered: {
                    root.hidden = false
                    root.currentIndex = index
                }
            }
            onObjectAdded:   (index, object) => viewMenu.insertItem(index, object)
            onObjectRemoved: (index, object) => viewMenu.removeItem(object)
        }

        MenuSeparator {}

        CustomMenuItem {
            text: qsTr("Hide")
            checkable: true
            autoExclusive: true
            checked: root.hidden
            onTriggered: root.hidden = !root.hidden
        }
    }
}
