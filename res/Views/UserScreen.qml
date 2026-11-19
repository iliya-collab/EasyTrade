import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Application.Core 1.0
import Components.Crypto 1.0
import Theme 1.0

Rectangle {
    id: root
    color: Theme.windowColor

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

        // --- Левая колонка: информация об аккаунте и ключе ---
        AccountInfoWidget {
            SplitView.preferredWidth: mainLayout.width * 0.30
            SplitView.minimumWidth: 320
        }

        // --- Правая колонка: кашелек и активы ---
        WalletInfoWidget {
            SplitView.fillWidth: true
            SplitView.minimumWidth: 300
        }

    }
}