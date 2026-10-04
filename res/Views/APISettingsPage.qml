import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import Components.Custom 1.0
import Theme 1.0
import Application.Core 1.0

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.margins
        spacing: Theme.spacing

        CustomLabel { text: "Active API" }

        CustomComboBox {
            id: activeCombo
            Layout.fillWidth: true
            model: ConfigurationManager.apiNames
            currentIndex: Math.max(0, model.indexOf(ConfigurationManager.activeApi))
            onActivated: ConfigurationManager.activeApi = currentText
        }

        CustomLabel { text: "All APIs" }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"
            border.width: Theme.borderWidth
            border.color: Theme.borderColor

            ListView {
                id: apiList
                anchors.fill: parent
                anchors.margins: Theme.margins
                clip: true
                spacing: 2
                model: ConfigurationManager.apiNames

                delegate: Rectangle {
                    width: apiList.width
                    height: 36
                    color: modelData === ConfigurationManager.activeApi
                           ? Theme.hoverColor
                           : "transparent"

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: Theme.margins
                        anchors.rightMargin: Theme.margins
                        spacing: Theme.spacing

                        CustomText {
                            Layout.fillWidth: true
                            text: modelData
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }

                        CustomButton {
                            text: "delete"
                            enabled: modelData !== ConfigurationManager.activeApi
                            onClicked: {
                                if (!ConfigurationManager.removeApi(modelData))
                                    console.warn("Failed to delete:", modelData)
                            }
                        }
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true

            CustomButton {
                text: "Create..."
                onClicked: {
                    apiCreationFormWindow.active = true
                }
            }

            CustomButton {
                text: "Save"
                onClicked: ConfigurationManager.saveConfig()
            }
        }
    }

    Connections {
        target: ConfigurationManager

        function onApisChanged() {}

        function onActiveApiChanged() {
            activeCombo.currentIndex = Math.max(
                0, ConfigurationManager.apiNames.indexOf(ConfigurationManager.activeApi))
        }
    }

    Loader {
        id: apiCreationFormWindow
        active: false
        source: "APICreationWindow.qml"

        Connections {
            target: apiCreationFormWindow.item
            function onVisibleChanged() {
                if (apiCreationFormWindow.item && !apiCreationFormWindow.item.visible)
                    apiCreationFormWindow.active = false
            }
        }
    }
}
