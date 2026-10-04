import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import Application.Core 1.0
import Components.Custom 1.0
import Theme 1.0

Window {
    id: root
    visible: true
    width: 700
    height: 400
    title: "Logs"
    color: Theme.windowColor

    readonly property var logModel: LogFilterProxyModel {
        sourceModel: AppCore.logModel
        logLevel: ""
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 6
        spacing: 6

        RowLayout {
            CustomTabBar {
                modelTabs: [
                    { text: "All",         level: "" },
                    { text: "Errors",      level: "error" },
                    { text: "Warnings",    level: "warning" },
                    { text: "Info",        level: "info" }
                ]
                onCurrentIndexChanged: {
                    var logLevel = modelTabs[currentIndex].level
                    logModel.logLevel = logLevel
                }
            }
            Item { Layout.fillWidth: true }
            CustomButton {
                text: "Clear"
                onClicked: if (AppCore.logModel) AppCore.logModel.clear()
            }
        }

        ListView {
            id: list
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: logModel
            property bool stick: true

            ScrollBar.vertical: ScrollBar {}

            onMovementEnded: stick = atYEnd
            onCountChanged: if (stick) Qt.callLater(positionViewAtEnd)

            delegate: Text {
                required property string time
                required property string level
                required property string category
                required property string message

                height: implicitHeight
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