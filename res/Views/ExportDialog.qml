import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs
import Components.Custom 1.0
import Application.Core 1.0

Dialog {
    id: root
    modal: true
    anchors.centerIn: Overlay.overlay
    width: 460
    standardButtons: Dialog.Ok | Dialog.Cancel

    property string defaultName: "export"
    // Специфичное содержимое (диапазон свечей и т.п.)
    default property alias extraContent: extraHolder.data

    signal exportRequested(string path, int format, var opts)

    ListModel {
        id: formatsModel

        ListElement { text: "JSON"; value: ExportFormat.Json; ext: "json" }
        ListElement { text: "XML";  value: ExportFormat.Xml;  ext: "xml"  }
        ListElement { text: "CSV";  value: ExportFormat.Csv;  ext: "csv"  }
        ListElement { text: "YAML"; value: ExportFormat.Yaml; ext: "yaml" }
    }

    readonly property var fmt: formatsModel.get(Math.max(0, formatBox.currentIndex))

    function buildOpts()
    {
        switch (fmt.value)
        {
            case ExportFormat.Json:
            {
                let o = { pretty: jsonPretty.checked }
                if (jsonRoot.text.length)
                    o.rootName = jsonRoot.text
                return o
            }
            case ExportFormat.Xml:
                return { rootName: xmlRoot.text, rowName: xmlRow.text, indent: xmlIndent.value }
            case ExportFormat.Csv:
                return { delimiter: csvDelim.text, withHeader: csvHeader.checked }
            case ExportFormat.Yaml:
            {
                let o = { indentSize: yamlIndent.value }
                if (yamlRoot.text.length)
                    o.rootName = yamlRoot.text
                return o
            }
        }
    }

    function withExtension(p)
    {
        return p.toLowerCase().endsWith("." + fmt.ext) ? p : p + "." + fmt.ext
    }

    onAboutToShow: pathField.text = ""
    Component.onCompleted: standardButton(Dialog.Ok).enabled = Qt.binding(() => pathField.text.length > 0)

    onAccepted: exportRequested(withExtension(pathField.text), fmt.value, buildOpts())

    FileDialog {
        id: fileDialog
        fileMode: FileDialog.SaveFile
        currentFile: "file:///" + root.defaultName
        nameFilters: [root.fmt.text + " (*." + root.fmt.ext + ")"]
        onAccepted: pathField.text = selectedFile.toString()
    }

    contentItem: ColumnLayout {
        spacing: 10

        // --- специфичное содержимое ---
        ColumnLayout { id: extraHolder; Layout.fillWidth: true }

        // --- формат ---
        RowLayout {
            CustomLabel {
                text: qsTr("Format")
                Layout.preferredWidth: 80
            }
            CustomComboBox {
                id: formatBox
                Layout.fillWidth: true
                model: formatsModel
                textRole: "text"
            }
        }

        // --- файл ---
        RowLayout {
            CustomLabel {
                text: qsTr("File")
                Layout.preferredWidth: 80
            }
            CustomTextField {
                id: pathField;
                Layout.fillWidth: true
                readOnly: true
            }
            CustomButton {
                text: "…";
                onClicked: fileDialog.open()
            }
        }

        // --- опции формата ---
        StackLayout {
            Layout.fillWidth: true
            currentIndex: formatBox.currentIndex

            // --- JSON ---
            GridLayout {
                columns: 2
                CustomLabel {
                    text: "rootName"
                }
                CustomTextField {
                    id: jsonRoot;
                    Layout.fillWidth: true;
                    placeholderText: qsTr("empty = plain array")
                }
                CustomCheckBox {
                    id: jsonPretty;
                    text: "pretty";
                    checked: true;
                    Layout.columnSpan: 2
                }
            }

            // --- XML ---
            GridLayout {
                columns: 2
                CustomLabel { text: "rootName" }
                CustomTextField { id: xmlRoot; text: "items"; Layout.fillWidth: true }
                CustomLabel { text: "rowName" }
                CustomTextField { id: xmlRow; text: "item"; Layout.fillWidth: true }
                CustomLabel { text: "indent" }
                SpinBox { id: xmlIndent; from: 0; to: 16; value: 4 }
            }

            // --- CSV ---
            GridLayout {
                columns: 2
                CustomLabel { text: "delimiter" }
                CustomTextField { id: csvDelim; text: ","; maximumLength: 1; Layout.preferredWidth: 50 }
                CustomCheckBox { id: csvHeader; text: "withHeader"; checked: true; Layout.columnSpan: 2 }
            }

            // --- YAML ---
            GridLayout {
                columns: 2
                CustomLabel { text: "rootName" }
                CustomTextField { id: yamlRoot; Layout.fillWidth: true; placeholderText: qsTr("empty = plain list") }
                CustomLabel { text: "indentSize" }
                SpinBox { id: yamlIndent; from: 1; to: 16; value: 2 }
            }
        }
    }
}