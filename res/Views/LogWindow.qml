import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import Application.Core 1.0
import Theme 1.0

Window {
    id: root
    visible: true
    width: 700
    height: 400
    title: "Logs"
    color: Theme.windowColor

    property int filter: 0   // 0 - все, 1 - ошибки, 2 - уведомления

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 6
        spacing: 6

        RowLayout {
            ComboBox {
                model: ["All", "Errors", "Info"]
                onCurrentIndexChanged: root.filter = currentIndex
            }
            Item { Layout.fillWidth: true }
            Button {
                text: "Clear"
                onClicked: if (root.logModel) root.logModel.clear()
            }
        }

        ListView {
            id: list
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: AppCore.logModel
            property bool stick: true

            ScrollBar.vertical: ScrollBar {}

            onMovementEnded: stick = atYEnd
            onCountChanged: if (stick) Qt.callLater(positionViewAtEnd)

            delegate: Text {
                required property string time
                required property string level
                required property string category
                required property string message

                readonly property bool shown:
                    root.filter === 0
                    || (root.filter === 1 && level === "error")
                    || (root.filter === 2 && level === "info")

                visible: shown
                height: shown ? implicitHeight : 0
                width: ListView.view.width
                wrapMode: Text.WordWrap
                color:  level === "error" ? "#ff5555" :
                        level === "warning" ? "#FFD700"
                        : "#dddddd"
                text: "[" + time + "] " + category + ": " + message
            }
        }
    }
}