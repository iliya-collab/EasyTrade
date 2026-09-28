/*import QtQuick 2.15
import QtQuick.Layouts 1.15
import Application.Core 1.0
import Components.Custom 1.0
import Theme 1.0

Rectangle {
    id: root
    color: Theme.windowColor

    property Api apiData: AppCore.accountState.api

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                Layout.fillHeight: true

                RowLayout {
                    CustomLabel {
                        text: "API key:"
                        Layout.margins: Theme.margins
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: Theme.fontSizeSmall
                    }
                    CustomTextField {
                        id: txtfAPIKey
                        Layout.margins: Theme.margins
                        text: apiData.apiKey
                        validator: RegularExpressionValidator {
                            regularExpression: /^[a-zA-Z0-9]{18,20}$/
                        }
                    }
                }

                RowLayout {
                    CustomLabel {
                        text: "Secret API key:"
                        Layout.margins: Theme.margins
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: Theme.fontSizeSmall
                    }
                    CustomTextField {
                        id: txtfSecretAPI
                        Layout.margins: Theme.margins
                        text: apiData.secretKey
                        validator: RegularExpressionValidator {
                            regularExpression: /^[a-zA-Z0-9]{32,36}$/
                        }
                    }
                }

                RowLayout {
                    CustomLabel {
                        text: "Type network:"
                        horizontalAlignment: Text.AlignHCenter
                        Layout.margins: Theme.margins
                        font.pixelSize: Theme.fontSizeSmall
                    }
                    CustomCheckBox {
                        id: chbTNetwork
                        Layout.margins: Theme.margins
                        text: "Testnet"
                        checked: apiData.isTestnet
                    }
                }
            } // Column

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight | Qt.AlignTop
                Layout.margins: Theme.margins
                color: "transparent"
                border.width: Theme.borderWidth
                border.color: Theme.borderColor

                ListView {
                    id: viewAssets
                    anchors.fill: parent
                    anchors.margins: Theme.margins
                    model: AppCore.accountState.assets
                    clip: true
                    reuseItems: true

                    delegate: Row {
                        spacing: Theme.spacing
                        width: viewAssets.width

                        CustomLabel {
                            text: model.name
                            width: (parent.width - 2 * parent.spacing) / 3
                            horizontalAlignment: Text.AlignHCenter
                        }
                        CustomLabel {
                            text: model.amount
                            width: (parent.width - 2 * parent.spacing) / 3
                            horizontalAlignment: Text.AlignHCenter
                        }
                        CustomLabel {
                            text: model.value
                            width: (parent.width - 2 * parent.spacing) / 3
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }
            }

        }

        CustomButton {
            text: "Connect"
            Layout.margins: Theme.margins
            Layout.alignment: Qt.AlignRight | Qt.AlignBottom
            enabled: txtfAPIKey.acceptableInput && txtfSecretAPI.acceptableInput
            onClicked: {
                AppCore.accountService.loadAccountBalance
            }
        }
    } // ColumnLayout
}*/

import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Application.Core 1.0
import Components.Custom 1.0
import Theme 1.0

Rectangle {
    id: root
    color: Theme.windowColor

    property Api apiData: AppCore.accountState.api

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.margins
        spacing: Theme.spacing

        // ─────────────────────────────────────────────
        // Верхняя часть: ключи слева, активы справа
        // ─────────────────────────────────────────────
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: Theme.spacing

            // ══ Левая колонка: ключи + info ══
            ColumnLayout {
                Layout.preferredWidth: 380
                Layout.maximumWidth: 420
                Layout.fillHeight: true
                spacing: Theme.spacing

                // --- Блок: API credentials ---
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: credColumn.implicitHeight + 2 * Theme.padding
                    color: "transparent"
                    border.color: Theme.borderColor
                    border.width: Theme.borderWidth
                    radius: Theme.radius

                    ColumnLayout {
                        id: credColumn
                        anchors.fill: parent
                        anchors.margins: Theme.padding
                        spacing: Theme.spacing

                        CustomLabel {
                            text: "API credentials"
                            font.pixelSize: Theme.fontSizeBody
                            font.bold: true
                            color: Theme.accentColor
                        }

                        GridLayout {
                            Layout.fillWidth: true
                            columns: 2
                            columnSpacing: Theme.spacing
                            rowSpacing: Theme.spacing

                            CustomLabel {
                                text: "API key:"
                                Layout.preferredWidth: 110
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.textColor
                            }
                            CustomTextField {
                                id: txtfAPIKey
                                Layout.fillWidth: true
                                text: root.apiData.apiKey
                                validator: RegularExpressionValidator {
                                    regularExpression: /^[a-zA-Z0-9]{18,20}$/
                                }
                            }

                            CustomLabel {
                                text: "Secret key:"
                                Layout.preferredWidth: 110
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.textColor
                            }
                            CustomTextField {
                                id: txtfSecretAPI
                                Layout.fillWidth: true
                                text: root.apiData.secretKey
                                echoMode: TextInput.Password
                                validator: RegularExpressionValidator {
                                    regularExpression: /^[a-zA-Z0-9]{32,36}$/
                                }
                            }
                        }

                        CustomButton {
                            text: "Connect"
                            Layout.alignment: Qt.AlignRight
                            implicitHeight: 30
                            enabled: txtfAPIKey.acceptableInput && txtfSecretAPI.acceptableInput
                            onClicked: {
                                root.apiData.apiKey = txtfAPIKey.text
                                root.apiData.secretKey = txtfSecretAPI.text
                                root.apiData.isTestnet = chbTNetwork.checked
                                AppCore.accountService.init(root.apiData)
                                AppCore.accountService.loadInfoAboutApi()
                                AppCore.accountService.loadAccountBalance()
                            }
                        }
                    }
                }

                // --- Блок: Key permissions ---
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: infoColumn.implicitHeight + 2 * Theme.padding
                    color: "transparent"
                    border.color: Theme.borderColor
                    border.width: Theme.borderWidth
                    radius: Theme.radius

                    ColumnLayout {
                        id: infoColumn
                        anchors.fill: parent
                        anchors.margins: Theme.padding
                        spacing: Theme.spacing

                        CustomLabel {
                            text: "Key permissions"
                            font.pixelSize: Theme.fontSizeBody
                            font.bold: true
                            color: Theme.accentColor
                        }

                        GridLayout {
                            Layout.fillWidth: true
                            columns: 2
                            columnSpacing: Theme.spacing
                            rowSpacing: Theme.spacing / 2

                            // helper-компонент для строки "label: value"
                            component PermRow: CustomLabel {
                                Layout.preferredWidth: 150
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.textColor
                            }
                            component BoolVal: CustomLabel {
                                font.pixelSize: Theme.fontSizeSmall
                                font.bold: true
                            }

                            // Read only
                            PermRow { text: "Read only:" }
                            BoolVal {
                                text: root.apiData.info.readOnly ? "yes" : "no"
                                color: root.apiData.info.readOnly ? Theme.accentColor : "#e05252"
                            }

                            // Spot trade
                            PermRow { text: "Spot trade:" }
                            BoolVal {
                                text: root.apiData.info.permissionSpotTrade ? "allowed" : "denied"
                                color: root.apiData.info.permissionSpotTrade ? "#4caf50" : "#e05252"
                            }

                            // Order contract
                            PermRow { text: "Order contract:" }
                            BoolVal {
                                text: root.apiData.info.permissionOrderContract ? "allowed" : "denied"
                                color: root.apiData.info.permissionOrderContract ? "#4caf50" : "#e05252"
                            }

                            // Position contract
                            PermRow { text: "Position contract:" }
                            BoolVal {
                                text: root.apiData.info.permissionPositionContract ? "allowed" : "denied"
                                color: root.apiData.info.permissionPositionContract ? "#4caf50" : "#e05252"
                            }

                            // Options trade
                            PermRow { text: "Options trade:" }
                            BoolVal {
                                text: root.apiData.info.permissionOptionsTrade ? "allowed" : "denied"
                                color: root.apiData.info.permissionOptionsTrade ? "#4caf50" : "#e05252"
                            }

                            // Withdraw
                            PermRow { text: "Withdraw:" }
                            BoolVal {
                                text: root.apiData.info.permissionWithdraw ? "allowed" : "denied"
                                color: root.apiData.info.permissionWithdraw ? "#4caf50" : "#e05252"
                            }

                            // Account transfer
                            PermRow { text: "Account transfer:" }
                            BoolVal {
                                text: root.apiData.info.permissionAccountTransfer ? "allowed" : "denied"
                                color: root.apiData.info.permissionAccountTransfer ? "#4caf50" : "#e05252"
                            }

                            // Sub transfer
                            PermRow { text: "Sub transfer:" }
                            BoolVal {
                                text: root.apiData.info.permissionSubTransfer ? "allowed" : "denied"
                                color: root.apiData.info.permissionSubTransfer ? "#4caf50" : "#e05252"
                            }

                            // Exchange (convert)
                            PermRow { text: "Exchange:" }
                            BoolVal {
                                text: root.apiData.info.permissionExchange ? "allowed" : "denied"
                                color: root.apiData.info.permissionExchange ? "#4caf50" : "#e05252"
                            }

                            // Earn
                            PermRow { text: "Earn:" }
                            BoolVal {
                                text: root.apiData.info.permissionEarn ? "allowed" : "denied"
                                color: root.apiData.info.permissionEarn ? "#4caf50" : "#e05252"
                            }
                        }

                        // ── Разделитель ──
                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 1
                            color: Theme.borderColor
                        }

                        // ── Блок: Account info ──
                        CustomLabel {
                            text: "Account info"
                            font.pixelSize: Theme.fontSizeBody
                            font.bold: true
                            color: Theme.accentColor
                        }

                        GridLayout {
                            Layout.fillWidth: true
                            columns: 2
                            columnSpacing: Theme.spacing
                            rowSpacing: Theme.spacing / 2

                            component InfoRow: CustomLabel {
                                Layout.preferredWidth: 150
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.textColor
                            }
                            component InfoVal: CustomLabel {
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.textColor
                            }

                            // Note
                            InfoRow { text: "Note:" }
                            InfoVal {
                                text: root.apiData.info.note.length > 0
                                      ? root.apiData.info.note : "—"
                            }

                            // User ID
                            InfoRow { text: "User ID:" }
                            InfoVal {
                                text: root.apiData.info.userId.length > 0
                                      ? root.apiData.info.userId : "—"
                            }

                            // VIP level
                            InfoRow { text: "VIP level:" }
                            InfoVal { text: root.apiData.info.vipLevel }

                            // KYC level
                            InfoRow { text: "KYC level:" }
                            InfoVal { text: root.apiData.info.kycLevel }

                            // Account type
                            InfoRow { text: "Account type:" }
                            InfoVal {
                                text: root.apiData.info.isUnifiedAccount
                                      ? "Unified (UTA)" : "Classic"
                            }

                            // Master / Sub
                            InfoRow { text: "Account role:" }
                            InfoVal {
                                text: root.apiData.info.isMaster ? "Master" : "Sub-account"
                            }

                            // Created at
                            InfoRow { text: "Created at:" }
                            InfoVal {
                                text: root.apiData.info.createdAt.length > 0
                                      ? root.apiData.info.createdAt : "—"
                            }

                            // Expired at
                            InfoRow { text: "Expired at:" }
                            InfoVal {
                                text: root.apiData.info.expiredAt.length > 0
                                      ? root.apiData.info.expiredAt : "—"
                            }

                            // Deadline days
                            InfoRow { text: "Days to expire:" }
                            InfoVal {
                                text: root.apiData.info.deadlineDay >= 0
                                      ? root.apiData.info.deadlineDay.toString() : "—"
                            }

                            // isPublic (helper)
                            InfoRow { text: "Public key:" }
                            InfoVal {
                                text: root.apiData.info.isPublic ? "yes" : "no"
                                font.bold: true
                                color: root.apiData.info.isPublic ? "#e05252" : "#4caf50"
                            }

                            // isDangerous (helper)
                            InfoRow { text: "Dangerous:" }
                            InfoVal {
                                text: root.apiData.info.isDangerous ? "yes" : "no"
                                font.bold: true
                                color: root.apiData.info.isDangerous ? "#e05252" : "#4caf50"
                            }

                            // canTrade (helper)
                            InfoRow { text: "Can trade:" }
                            InfoVal {
                                text: root.apiData.info.canTrade ? "yes" : "no"
                                font.bold: true
                                color: root.apiData.info.canTrade ? "#4caf50" : "#e05252"
                            }
                        }

                        // ── Allowed IPs ──
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: Theme.spacing / 2

                            CustomLabel {
                                text: "Allowed IPs:"
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.textColor
                            }

                            CustomLabel {
                                visible: root.apiData.info.ips.length === 0
                                text: "Any IP"
                                font.pixelSize: Theme.fontSizeSmall
                                font.italic: true
                                color: Theme.textColor
                            }

                            Flow {
                                Layout.fillWidth: true
                                spacing: Theme.spacing / 2
                                visible: root.apiData.info.ips.length > 0

                                Repeater {
                                    model: root.apiData.info.ips
                                    delegate: Rectangle {
                                        color: Theme.buttonColor
                                        radius: Theme.radius / 2
                                        border.color: Theme.borderColor
                                        border.width: Theme.borderWidth
                                        implicitWidth: ipText.implicitWidth + Theme.padding
                                        implicitHeight: ipText.implicitHeight + Theme.padding / 2

                                        CustomLabel {
                                            id: ipText
                                            anchors.centerIn: parent
                                            text: modelData
                                            font.pixelSize: Theme.fontSizeSmall
                                            color: Theme.textColor
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                Item { Layout.fillHeight: true }
            }

            // ══ Правая колонка: активы ══
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 320
                color: "transparent"
                border.color: Theme.borderColor
                border.width: Theme.borderWidth
                radius: Theme.radius

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.padding
                    spacing: Theme.spacing / 2

                    CustomLabel {
                        text: "Assets"
                        font.pixelSize: Theme.fontSizeBody
                        font.bold: true
                        color: Theme.accentColor
                    }

                    // Общие ширины колонок
                    QtObject {
                        id: cols
                        readonly property int nameWidth: 100
                        readonly property int amountWidth: 160
                        readonly property int valueWidth: 180
                    }

                    // Заголовки
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: Theme.spacing

                        CustomLabel {
                            text: "Name"
                            Layout.preferredWidth: cols.nameWidth
                            font.pixelSize: Theme.fontSizeBody
                            font.bold: true
                            color: Theme.textColor
                        }
                        CustomLabel {
                            text: "Amount"
                            Layout.preferredWidth: cols.amountWidth
                            horizontalAlignment: Text.AlignRight
                            font.pixelSize: Theme.fontSizeBody
                            font.bold: true
                            color: Theme.textColor
                        }
                        CustomLabel {
                            text: "Value"
                            Layout.preferredWidth: cols.valueWidth
                            horizontalAlignment: Text.AlignRight
                            font.pixelSize: Theme.fontSizeBody
                            font.bold: true
                            color: Theme.textColor
                        }
                        Item { Layout.fillWidth: true }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 1
                        color: Theme.borderColor
                    }

                    ListView {
                        id: viewAssets
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        reuseItems: true
                        model: AppCore.accountState.assets

                        delegate: RowLayout {
                            width: viewAssets.width
                            spacing: Theme.spacing

                            CustomLabel {
                                text: model.name
                                Layout.preferredWidth: cols.nameWidth
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.textColor
                            }
                            CustomLabel {
                                text: Number(model.value).toLocaleString(Qt.locale(), 'f', 2)
                                Layout.preferredWidth: cols.amountWidth
                                horizontalAlignment: Text.AlignRight
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.textColor
                            }
                            CustomLabel {
                                text: Number(model.value).toLocaleString(Qt.locale(), 'f', 2)
                                Layout.preferredWidth: cols.valueWidth
                                horizontalAlignment: Text.AlignRight
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.textColor
                            }
                            Item { Layout.fillWidth: true }
                        }
                    }
                }

            }
        }
    }
}
