import QtQuick 2.15
import QtQuick.Layouts 2.15
import Application.Core 1.0
import Components.Configs 1.0
import Components.Custom 1.0

Item {
    id: root

    readonly property var executionModel: AppCore.accountState.executions

    readonly property int rowHeight: TradeHistoryConfig.rowHeight

    readonly property var columns: TradeHistoryConfig.columns

    visible: executionModel !== null

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: TradeHistoryConfig.margins

        // Заголовок таблицы
        RowLayout {
            Layout.fillWidth: true
            Layout.leftMargin: TradeHistoryConfig.rowHorizontalMargin
            Layout.rightMargin: TradeHistoryConfig.rowHorizontalMargin
            spacing: TradeHistoryConfig.rowSpacing

            Repeater {
                model: root.columns
                Text {
                    required property var modelData
                    text: modelData.title
                    color: TradeHistoryConfig.textSecondary
                    font.pixelSize: TradeHistoryConfig.headerFontSize
                    horizontalAlignment: Text.AlignLeft
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                    Layout.preferredWidth: modelData.width
                    Layout.alignment: Qt.AlignVCenter
                }
            }
        }

        ListView {
            id: viewExecutions
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            reuseItems: true
            model: root.executionModel
            delegate: ExecutionDelegate {
                width: viewExecutions.width
                height: root.rowHeight
                columns: root.columns
            }
        }
    }

    // ---  ExecutionDelegate   ---
    component ExecutionDelegate: Item {
        id: execDel

        property var columns: []

        required property string category
        required property string symbol
        required property string side
        required property string execValue
        required property string execPrice
        required property string execQty
        required property string execType
        required property string execFee
        required property string feeCurrency
        required property string execId
        required property var execTime

        function formatTime(ms) { return TradeHistoryConfig.formatTime(ms) }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: TradeHistoryConfig.rowHorizontalMargin
            anchors.rightMargin: TradeHistoryConfig.rowHorizontalMargin
            spacing: TradeHistoryConfig.rowSpacing

            Value { text: category;               Layout.preferredWidth: execDel.columns[0].width }
            Value { text: symbol;                 Layout.preferredWidth: execDel.columns[1].width }
            Value {
                text: side
                color: TradeHistoryConfig.sideColor(side)
                Layout.preferredWidth: execDel.columns[2].width
            }
            Value { text: execValue;              Layout.preferredWidth: execDel.columns[3].width }
            Value { text: execPrice;              Layout.preferredWidth: execDel.columns[4].width }
            Value { text: execQty;                Layout.preferredWidth: execDel.columns[5].width }
            Value { text: execType;               Layout.preferredWidth: execDel.columns[6].width }
            Value { text: TradeHistoryConfig.formatFee(execFee, feeCurrency); Layout.preferredWidth: execDel.columns[7].width }
            Value { text: execId;                 Layout.preferredWidth: execDel.columns[8].width }
            Value { text: execDel.formatTime(execTime); Layout.preferredWidth: execDel.columns[9].width }
        }
    }

    component Value: CustomText {
        color: TradeHistoryConfig.textSecondary
        font.family: TradeHistoryConfig.cellFontFamily
        font.pixelSize: TradeHistoryConfig.cellFontSize
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        elide: Text.ElideRight
    }
}