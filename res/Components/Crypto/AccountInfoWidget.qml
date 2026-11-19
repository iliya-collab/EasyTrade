import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Application.Core 1.0
import Components.Custom 1.0
import Theme 1.0

Rectangle {
    id: root
    color: "transparent"
    clip: true

    readonly property var apiData: AppCore.accountState.apiController

    ScrollView {
        id: scroll
        anchors.fill: parent
        anchors.margins: Theme.padding
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

        ColumnLayout {
            id: infoColumn
            width: scroll.availableWidth
            spacing: Theme.spacing

            component InfoLine: RowLayout {
                Layout.fillWidth: true
                spacing: Theme.spacing

                property alias labelText: keyText.text
                property alias valueText: valText.text
                property alias valueColor: valText.color
                property alias valueBold: valText.font.bold

                CustomLabel {
                    id: keyText
                    Layout.fillWidth: true
                    Layout.preferredWidth: 55
                    Layout.minimumWidth: 0
                    elide: Text.ElideRight
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.textColor
                }
                CustomLabel {
                    id: valText
                    Layout.fillWidth: true
                    Layout.preferredWidth: 45
                    Layout.minimumWidth: 0
                    elide: Text.ElideRight
                    font.pixelSize: Theme.fontSizeSmall
                }
            }

            CustomLabel {
                text: "Key permissions"
                font.pixelSize: Theme.fontSizeBody
                font.bold: true
                color: Theme.accentColor
            }

            // Список permission-строк
            InfoLine {
                labelText: "Read only:"
                valueText: root.apiData.readOnly ? "yes" : "no"
                valueColor: root.apiData.readOnly ? Theme.accentColor : "#e05252"
                valueBold: true
            }
            InfoLine {
                labelText: "Spot trade:"
                valueText: root.apiData.permissionSpotTrade ? "allowed" : "denied"
                valueColor: root.apiData.permissionSpotTrade ? "#4caf50" : "#e05252"
                valueBold: true
            }
            InfoLine {
                labelText: "Order contract:"
                valueText: root.apiData.permissionOrderContract ? "allowed" : "denied"
                valueColor: root.apiData.permissionOrderContract ? "#4caf50" : "#e05252"
                valueBold: true
            }
            InfoLine {
                labelText: "Position contract:"
                valueText: root.apiData.permissionPositionContract ? "allowed" : "denied"
                valueColor: root.apiData.permissionPositionContract ? "#4caf50" : "#e05252"
                valueBold: true
            }
            InfoLine {
                labelText: "Options trade:"
                valueText: root.apiData.permissionOptionsTrade ? "allowed" : "denied"
                valueColor: root.apiData.permissionOptionsTrade ? "#4caf50" : "#e05252"
                valueBold: true
            }
            InfoLine {
                labelText: "Withdraw:"
                valueText: root.apiData.permissionWithdraw ? "allowed" : "denied"
                valueColor: root.apiData.permissionWithdraw ? "#4caf50" : "#e05252"
                valueBold: true
            }
            InfoLine {
                labelText: "Account transfer:"
                valueText: root.apiData.permissionAccountTransfer ? "allowed" : "denied"
                valueColor: root.apiData.permissionAccountTransfer ? "#4caf50" : "#e05252"
                valueBold: true
            }
            InfoLine {
                labelText: "Sub transfer:"
                valueText: root.apiData.permissionSubTransfer ? "allowed" : "denied"
                valueColor: root.apiData.permissionSubTransfer ? "#4caf50" : "#e05252"
                valueBold: true
            }
            InfoLine {
                labelText: "Exchange:"
                valueText: root.apiData.permissionExchange ? "allowed" : "denied"
                valueColor: root.apiData.permissionExchange ? "#4caf50" : "#e05252"
                valueBold: true
            }
            InfoLine {
                labelText: "Earn:"
                valueText: root.apiData.permissionEarn ? "allowed" : "denied"
                valueColor: root.apiData.permissionEarn ? "#4caf50" : "#e05252"
                valueBold: true
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: Theme.borderColor
            }

            CustomLabel {
                text: "Account info"
                font.pixelSize: Theme.fontSizeBody
                font.bold: true
                color: Theme.accentColor
            }

            InfoLine {
                labelText: "Note:"
                valueText: root.apiData.note.length > 0 ? root.apiData.note : "—"
            }
            InfoLine {
                labelText: "User ID:"
                valueText: root.apiData.userId.length > 0 ? root.apiData.userId : "—"
            }
            InfoLine {
                labelText: "VIP level:"
                valueText: root.apiData.vipLevel
            }
            InfoLine {
                labelText: "KYC level:"
                valueText: root.apiData.kycLevel
            }
            InfoLine {
                labelText: "Account type:"
                valueText: root.apiData.isUnifiedAccount ? "Unified (UTA)" : "Classic"
            }
            InfoLine {
                labelText: "Account role:"
                valueText: root.apiData.isMaster ? "Master" : "Sub-account"
            }
            InfoLine {
                labelText: "Created at:"
                valueText: root.apiData.createdAt.length > 0 ? root.apiData.createdAt : "—"
            }
            InfoLine {
                labelText: "Expired at:"
                valueText: root.apiData.expiredAt.length > 0 ? root.apiData.expiredAt : "—"
            }
            InfoLine {
                labelText: "Days to expire:"
                valueText: root.apiData.deadlineDay >= 0 ? root.apiData.deadlineDay.toString() : "—"
            }
            InfoLine {
                labelText: "Public key:"
                valueText: root.apiData.isPublic ? "yes" : "no"
                valueColor: root.apiData.isPublic ? "#e05252" : "#4caf50"
                valueBold: true
            }
            InfoLine {
                labelText: "Dangerous:"
                valueText: root.apiData.isDangerous ? "yes" : "no"
                valueColor: root.apiData.isDangerous ? "#e05252" : "#4caf50"
                valueBold: true
            }
            InfoLine {
                labelText: "Can trade:"
                valueText: root.apiData.canTrade ? "yes" : "no"
                valueColor: root.apiData.canTrade ? "#4caf50" : "#e05252"
                valueBold: true
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: Theme.spacing / 2

                CustomLabel {
                    text: "Allowed IPs:"
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.textColor
                }

                CustomLabel {
                    visible: root.apiData.ips.length === 0
                    text: "Any IP"
                    font.pixelSize: Theme.fontSizeSmall
                    font.italic: true
                    color: Theme.textColor
                }

                Flow {
                    Layout.fillWidth: true
                    spacing: Theme.spacing / 2
                    visible: root.apiData.ips.length > 0

                    Repeater {
                        model: root.apiData.ips
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

}
