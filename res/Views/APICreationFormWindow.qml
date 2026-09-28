import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Theme 1.0
import Components.Custom 1.0
import Application.Core 1.0

Window {
    id: root
    width: 480
    height: 280
    visible: true
    title: "Create API"
    color: Theme.windowColor

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.margins
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

}
