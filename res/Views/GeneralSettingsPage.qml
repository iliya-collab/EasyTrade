import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls 2.15
import Components.Custom 1.0
import Theme 1.0
import Application.Core 1.0

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.margins
        spacing: Theme.spacing

        CustomLabel { text: "General" }

        CustomLabel { text: "Theme" }

        ListModel {
            id: themeModel
            ListElement { label: "System"; value: "system" }
            ListElement { label: "Dark";   value: "dark"   }
            ListElement { label: "Light";  value: "light"  }
        }

        CustomComboBox {
            id: themeCombo
            Layout.fillWidth: true

            model: themeModel
            textRole: "label"
            valueRole: "value"

            onActivated: Theme.mode = currentValue
        }

        CustomLabel {
            visible: Theme.mode === "system"
            text: "System theme: " + (Theme.systemIsDark ? "dark" : "light")
        }

        CustomSwitch {
            id: autoConnCheck
            text: "Auto connection"
            checked: ConfigurationManager.autoConnection
            onToggled: ConfigurationManager.autoConnection = checked
        }

        CustomSwitch {
            id: testnetCheck
            text: "Testnet"
            checked: ConfigurationManager.testnet
            onToggled: ConfigurationManager.testnet = checked
        }

        CustomSwitch {
            id: enableTradesCheck
            text: "Enable trades"
            checked: ConfigurationManager.enableTrades
            onToggled: ConfigurationManager.enableTrades = checked
        }

        Item { Layout.fillHeight: true }

        CustomButton {
            text: "Save"
            onClicked: ConfigurationManager.saveConfig()
        }
    }

    Component.onCompleted: {
        console.info("mode:", Theme.mode)
        console.info("idx:", themeCombo.indexOfValue(Theme.mode))
        console.info("count:", themeCombo.count)
    }

    Connections {
        target: Theme
        function onModeChanged() {
            themeCombo.currentIndex = themeCombo.indexOfValue(Theme.mode)
        }
    }

}
