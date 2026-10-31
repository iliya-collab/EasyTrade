import QtQuick
import QtQuick.Controls
import QtQml
import QtCore
import Theme 1.0
import Components.Crypto 1.0

// Один виджет, который переключает вложенные виджеты через контекстное меню (ПКМ).
// Чтобы добавить ещё один вид, достаточно добавить Component и запись в `views`.
Item {
    id: root

    // ===== Доступные виды =====
    readonly property var views: [
        { title: qsTr("Order book"),    component: orderbookComponent },
        { title: qsTr("Public trades"), component: tradesComponent    }
    ]

    // Индекс текущего вида; сохраняется между запусками
    property alias currentIndex: store.viewIndex
    readonly property int safeIndex: Math.max(0, Math.min(currentIndex, views.length - 1))

    Settings {
        id: store
        category: "MarketDataWidget"
        property int viewIndex: 0
    }

    // ===== Содержимое: активный вид (неактивные выгружаются) =====
    Loader {
        id: contentLoader
        anchors.fill: parent
        sourceComponent: root.views[root.safeIndex].component
    }

    Component { id: orderbookComponent; OrderbookWidget {} }
    Component { id: tradesComponent;    PublicTradesWidget {} }

    // ===== Контекстное меню (ПКМ) =====
    TapHandler {
        acceptedButtons: Qt.RightButton
        onTapped: viewMenu.popup(point.position.x, point.position.y)
    }

    Menu {
        id: viewMenu

        background: Rectangle {
            implicitWidth: 180
            color: Theme.windowColor
            border.width: Theme.borderWidth
            border.color: Theme.borderColor
        }

        Instantiator {
            model: root.views

            delegate: MenuItem {
                required property int index
                required property var modelData

                text: modelData.title
                checkable: true
                autoExclusive: true
                checked: root.safeIndex === index
                onTriggered: root.currentIndex = index
            }

            onObjectAdded: (index, object) => viewMenu.insertItem(index, object)
            onObjectRemoved: (index, object) => viewMenu.removeItem(object)
        }
    }
}
