import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Theme 1.0
import Components.Custom 1.0
import Application.Core 1.0

Dialog {
    id: root

    width: 480
    height: 280

    modal: true
    parent: Overlay.overlay
    anchors.centerIn: Overlay.overlay

    // Для формы с вводом данных: закрытие только по Esc или кнопке Cancel,
    // чтобы случайно не потерять введённое.
    closePolicy: Popup.CloseOnEscape

    // ---- Фон ----
    background: Rectangle {
        color: Theme.windowColor
        radius: 8
        border.color: "#3a3a4a"
        border.width: 1
    }

    // ---- Заголовок ----
    header: Item {
        implicitHeight: headerRow.implicitHeight + Theme.margins

        RowLayout {
            id: headerRow
            anchors.fill: parent
            anchors.margins: Theme.margins

            CustomLabel {
                text: "Create API"
                color: Theme.textColor
                font.family: Theme.fontFamily
                font.pixelSize: Theme.fontSizeHeader
                font.bold: true
            }

            Item { Layout.fillWidth: true }
        }
    }

    // ---- Контент ----
    contentItem: ColumnLayout {
        spacing: Theme.spacing

        GridLayout {
            Layout.fillWidth: true
            columns: 2
            columnSpacing: Theme.spacing
            rowSpacing: Theme.spacing

            CustomLabel {
                text: "Name"
                color: Theme.textColor
                font.family: Theme.fontFamily
                font.pixelSize: Theme.fontSizeBody
            }
            CustomTextField {
                id: nameField
                Layout.fillWidth: true
                placeholderText: "my-key"
                font.family: Theme.fontFamily
                font.pixelSize: Theme.fontSizeBody
            }

            CustomLabel {
                text: "API Key"
                color: Theme.textColor
                font.family: Theme.fontFamily
                font.pixelSize: Theme.fontSizeBody
            }
            CustomTextField {
                id: keyField
                Layout.fillWidth: true
                placeholderText: "Public key"
                font.family: Theme.fontFamily
                font.pixelSize: Theme.fontSizeBody
            }

            CustomLabel {
                text: "Secret Key"
                color: Theme.textColor
                font.family: Theme.fontFamily
                font.pixelSize: Theme.fontSizeBody
            }
            CustomTextField {
                id: secretField
                Layout.fillWidth: true
                placeholderText: "Secret"
                font.family: Theme.fontFamily
                font.pixelSize: Theme.fontSizeBody
                echoMode: TextInput.Password
            }
        }

        Item { Layout.fillHeight: true }

        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacing

            Item { Layout.fillWidth: true }

            CustomButton {
                text: "Add"
                enabled: nameField.text.length > 0
                      && keyField.text.length > 0
                      && secretField.text.length > 0
                onClicked: {
                    if (ConfigurationManager.addApi(nameField.text,
                                                    keyField.text,
                                                    secretField.text))
                    {
                        ConfigurationManager.saveConfig()
                        root.close()
                    }
                }
            }

            CustomButton {
                text: "Cancel"
                onClicked: root.close()
            }
        }
    }

    // ---- Сброс формы при каждом открытии ----
    onOpened: {
        nameField.text = ""
        keyField.text = ""
        secretField.text = ""
        nameField.forceActiveFocus()
    }
}