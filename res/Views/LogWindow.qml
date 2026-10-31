import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import Application.Core 1.0
import Components.Custom 1.0
import Components.Crypto 1.0
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

    property int appliedCount: 0

    function esc(s) {
        return s.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;")
    }

    function colorFor(level) {
        return level === "error"   ? "#ff5555"
             : level === "warning" ? "#FFD700"
             :                       "#dddddd"
    }

    function lineHtml(row) {
        return "<span style='color:" + colorFor(logModel.levelAt(row)) + "'>"
             + esc(logModel.lineAt(row)) + "</span>"
    }

    function isAtEnd() {
        var bar = scroll.ScrollBar.vertical
        return bar.position + bar.size >= 0.999
    }

    function scrollToEnd() {
        var bar = scroll.ScrollBar.vertical
        bar.position = 1.0 - bar.size
    }

    function rebuild() {
        var stick = isAtEnd()
        var parts = []
        var n = logModel.rowCount()
        for (var i = 0; i < n; ++i)
            parts.push(lineHtml(i))
        logView.text = parts.join("<br>")
        appliedCount = n
        if (stick) Qt.callLater(scrollToEnd)
    }

    function appendRows(first, last) {
        var stick = isAtEnd()
        for (var i = first; i <= last; ++i)
            logView.append(lineHtml(i))
        appliedCount = logModel.rowCount()
        if (stick) Qt.callLater(scrollToEnd)
    }

    Connections {
        target: root.logModel

        function onRowsInserted(parent, first, last) {
            if (first === root.appliedCount) root.appendRows(first, last)
            else root.rebuild()
        }
        function onRowsRemoved()   { root.rebuild() }
        function onModelReset()    { root.rebuild() }
        function onLayoutChanged() { root.rebuild() }
    }

    Component.onCompleted: rebuild()

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
                    Qt.callLater(root.rebuild)
                }
            }
            Item { Layout.fillWidth: true }
            PingIndicator {
                pingValue: AppCore.marketState.pingMs
            }
            CustomButton {
                text: "Clear"
                onClicked: if (AppCore.logModel) AppCore.logModel.clear()
            }
        }

        ScrollView {
            id: scroll
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            TextArea {
                id: logView
                readOnly: true
                selectByMouse: true
                persistentSelection: true
                wrapMode: TextEdit.WordWrap
                textFormat: TextEdit.RichText
                color: Theme.textColor
                selectionColor: Theme.selectColor
                selectedTextColor: Theme.selectTextColor
                background: null
            }

        }
    }
}