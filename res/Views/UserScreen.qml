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

    SplitView {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: Theme.margins
        orientation: Qt.Horizontal

        handle: Rectangle {
            implicitWidth: 6
            color: SplitHandle.pressed ? Theme.accentColor
                 : SplitHandle.hovered ? Qt.lighter(Theme.borderColor, 1.5)
                 : "transparent"

            Rectangle {
                anchors.centerIn: parent
                width: 1
                height: parent.height
                color: Theme.borderColor
            }
        }

        // ══ Левая колонка: информация о ключе ══
        Rectangle {
            id: leftPanel
            SplitView.preferredWidth: mainLayout.width * 0.30
            SplitView.minimumWidth: 320
            color: "transparent"
            border.color: Theme.borderColor
            border.width: Theme.borderWidth
            radius: Theme.radius
            clip: true

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

                    // ── Компонент: строка "label: value" ──
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

                    // ── Список permission-строк ──
                    InfoLine {
                        labelText: "Read only:"
                        valueText: root.apiData.info.readOnly ? "yes" : "no"
                        valueColor: root.apiData.info.readOnly ? Theme.accentColor : "#e05252"
                        valueBold: true
                    }
                    InfoLine {
                        labelText: "Spot trade:"
                        valueText: root.apiData.info.permissionSpotTrade ? "allowed" : "denied"
                        valueColor: root.apiData.info.permissionSpotTrade ? "#4caf50" : "#e05252"
                        valueBold: true
                    }
                    InfoLine {
                        labelText: "Order contract:"
                        valueText: root.apiData.info.permissionOrderContract ? "allowed" : "denied"
                        valueColor: root.apiData.info.permissionOrderContract ? "#4caf50" : "#e05252"
                        valueBold: true
                    }
                    InfoLine {
                        labelText: "Position contract:"
                        valueText: root.apiData.info.permissionPositionContract ? "allowed" : "denied"
                        valueColor: root.apiData.info.permissionPositionContract ? "#4caf50" : "#e05252"
                        valueBold: true
                    }
                    InfoLine {
                        labelText: "Options trade:"
                        valueText: root.apiData.info.permissionOptionsTrade ? "allowed" : "denied"
                        valueColor: root.apiData.info.permissionOptionsTrade ? "#4caf50" : "#e05252"
                        valueBold: true
                    }
                    InfoLine {
                        labelText: "Withdraw:"
                        valueText: root.apiData.info.permissionWithdraw ? "allowed" : "denied"
                        valueColor: root.apiData.info.permissionWithdraw ? "#4caf50" : "#e05252"
                        valueBold: true
                    }
                    InfoLine {
                        labelText: "Account transfer:"
                        valueText: root.apiData.info.permissionAccountTransfer ? "allowed" : "denied"
                        valueColor: root.apiData.info.permissionAccountTransfer ? "#4caf50" : "#e05252"
                        valueBold: true
                    }
                    InfoLine {
                        labelText: "Sub transfer:"
                        valueText: root.apiData.info.permissionSubTransfer ? "allowed" : "denied"
                        valueColor: root.apiData.info.permissionSubTransfer ? "#4caf50" : "#e05252"
                        valueBold: true
                    }
                    InfoLine {
                        labelText: "Exchange:"
                        valueText: root.apiData.info.permissionExchange ? "allowed" : "denied"
                        valueColor: root.apiData.info.permissionExchange ? "#4caf50" : "#e05252"
                        valueBold: true
                    }
                    InfoLine {
                        labelText: "Earn:"
                        valueText: root.apiData.info.permissionEarn ? "allowed" : "denied"
                        valueColor: root.apiData.info.permissionEarn ? "#4caf50" : "#e05252"
                        valueBold: true
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 1
                        color: Theme.borderColor
                    }

                    CustomLabel {
                        text: "Key info"
                        font.pixelSize: Theme.fontSizeBody
                        font.bold: true
                        color: Theme.accentColor
                    }

                    // ── Account info ──
                    InfoLine {
                        labelText: "Note:"
                        valueText: root.apiData.info.note.length > 0 ? root.apiData.info.note : "—"
                    }
                    InfoLine {
                        labelText: "User ID:"
                        valueText: root.apiData.info.userId.length > 0 ? root.apiData.info.userId : "—"
                    }
                    InfoLine {
                        labelText: "VIP level:"
                        valueText: root.apiData.info.vipLevel
                    }
                    InfoLine {
                        labelText: "KYC level:"
                        valueText: root.apiData.info.kycLevel
                    }
                    InfoLine {
                        labelText: "Account type:"
                        valueText: root.apiData.info.isUnifiedAccount ? "Unified (UTA)" : "Classic"
                    }
                    InfoLine {
                        labelText: "Account role:"
                        valueText: root.apiData.info.isMaster ? "Master" : "Sub-account"
                    }
                    InfoLine {
                        labelText: "Created at:"
                        valueText: root.apiData.info.createdAt.length > 0 ? root.apiData.info.createdAt : "—"
                    }
                    InfoLine {
                        labelText: "Expired at:"
                        valueText: root.apiData.info.expiredAt.length > 0 ? root.apiData.info.expiredAt : "—"
                    }
                    InfoLine {
                        labelText: "Days to expire:"
                        valueText: root.apiData.info.deadlineDay >= 0 ? root.apiData.info.deadlineDay.toString() : "—"
                    }
                    InfoLine {
                        labelText: "Public key:"
                        valueText: root.apiData.info.isPublic ? "yes" : "no"
                        valueColor: root.apiData.info.isPublic ? "#e05252" : "#4caf50"
                        valueBold: true
                    }
                    InfoLine {
                        labelText: "Dangerous:"
                        valueText: root.apiData.info.isDangerous ? "yes" : "no"
                        valueColor: root.apiData.info.isDangerous ? "#e05252" : "#4caf50"
                        valueBold: true
                    }
                    InfoLine {
                        labelText: "Can trade:"
                        valueText: root.apiData.info.canTrade ? "yes" : "no"
                        valueColor: root.apiData.info.canTrade ? "#4caf50" : "#e05252"
                        valueBold: true
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
        }

        // ══ Правая колонка: активы ══
        Rectangle {
            id: rightPanel
            SplitView.fillWidth: true
            SplitView.minimumWidth: 300
            color: "transparent"
            border.color: Theme.borderColor
            border.width: Theme.borderWidth
            radius: Theme.radius
            clip: true

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

                // Ширины колонок привязаны к ширине правой панели —
                // никакой циклической зависимости.
                QtObject {
                    id: cols
                    readonly property real spacing: Theme.spacing * 3
                    readonly property real total: Math.max(rightPanel.width - Theme.padding * 2 - spacing, 300)
                    readonly property real nameWidth:   total * 0.30
                    readonly property real amountWidth: total * 0.35
                    readonly property real valueWidth:  total * 0.35
                }

                // Заголовки
                RowLayout {
                    Layout.fillWidth: true
                    spacing: Theme.spacing

                    CustomLabel {
                        text: "Name"
                        Layout.preferredWidth: cols.nameWidth
                        Layout.fillWidth: true
                        Layout.minimumWidth: 0
                        elide: Text.ElideRight
                        font.pixelSize: Theme.fontSizeBody
                        font.bold: true
                        color: Theme.textColor
                    }
                    CustomLabel {
                        text: "Amount"
                        Layout.preferredWidth: cols.amountWidth
                        Layout.fillWidth: true
                        Layout.minimumWidth: 0
                        elide: Text.ElideRight
                        horizontalAlignment: Text.AlignRight
                        font.pixelSize: Theme.fontSizeBody
                        font.bold: true
                        color: Theme.textColor
                    }
                    CustomLabel {
                        text: "Value"
                        Layout.preferredWidth: cols.valueWidth
                        Layout.fillWidth: true
                        Layout.minimumWidth: 0
                        elide: Text.ElideRight
                        horizontalAlignment: Text.AlignRight
                        font.pixelSize: Theme.fontSizeBody
                        font.bold: true
                        color: Theme.textColor
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 1
                    color: Theme.borderColor
                }

                // ListView сам скроллится, занимает всё оставшееся место
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
                            Layout.fillWidth: true
                            Layout.minimumWidth: 0
                            elide: Text.ElideRight
                            font.pixelSize: Theme.fontSizeBody
                            color: Theme.textColor
                        }
                        CustomLabel {
                            text: Number(model.value).toLocaleString(Qt.locale(), 'f', 2)
                            Layout.preferredWidth: cols.amountWidth
                            Layout.fillWidth: true
                            Layout.minimumWidth: 0
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignRight
                            font.pixelSize: Theme.fontSizeBody
                            color: Theme.textColor
                        }
                        CustomLabel {
                            text: Number(model.value).toLocaleString(Qt.locale(), 'f', 2)
                            Layout.preferredWidth: cols.valueWidth
                            Layout.fillWidth: true
                            Layout.minimumWidth: 0
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignRight
                            font.pixelSize: Theme.fontSizeBody
                            color: Theme.textColor
                        }
                    }
                }
            }
        }
    }
}