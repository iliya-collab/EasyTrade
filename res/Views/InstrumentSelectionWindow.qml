import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Application.Core 1.0
import Components.Custom 1.0
import Components.Crypto 1.0
import Theme 1.0

Window {

    id: root

    width: 500
    height: 420
    minimumWidth: 280
    minimumHeight: 320
    visible: true
    title: "Select trade symbol"
    color: Theme.windowColor

    QtObject
    {
        id: internal

        property tInstrumentInfo selectedInstrument
        property bool instrumentSelected: false
        readonly property string selectedCategory: selectedInstrument ? EnumHelpers.marketTypeToApiString(selectedInstrument.category) : "-"
        readonly property string selectedSymbol: selectedInstrument ? selectedInstrument.symbol : "-"
        readonly property string selectedBaseCoin: selectedInstrument ? selectedInstrument.baseCoin : "-"
        readonly property string selectedQuoteCoin: selectedInstrument ? selectedInstrument.quoteCoin : "-"
        readonly property string selectedStatus: selectedInstrument ? selectedInstrument.status : "-"
        readonly property string selectedId: selectedInstrument ? selectedInstrument.id : ""
        property bool advancedOpen: false

        function selectInstrument(inst)
        {
            selectedInstrument = inst
            instrumentSelected = true
        }

        function confirmSelection(inst)
        {
            if (!instrumentSelected)
                return

            root.currentInstrumentSelected(inst)
        }

        function confirmOpen(inst)
        {
            if (!instrumentSelected)
                return

            root.instrumentOpened(inst)
        }

        function confirmClose(inst)
        {
            if (!instrumentSelected)
                return

            root.instrumentClosed(inst)
        }
    }

    signal currentInstrumentSelected(tInstrumentInfo inst)
    signal instrumentOpened(tInstrumentInfo inst)
    signal instrumentClosed(tInstrumentInfo inst)

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.margins

        // --- Заголовок ---
        CustomTabBar {
            id: tabsBar
            modelTabs: [
                { text: "spot",     type: EnumHelpers.MarketType.Spot },
                { text: "linear",   type: EnumHelpers.MarketType.Linear }
            ]
            onCurrentIndexChanged: {
                internal.instrumentSelected = false
                availableInsts.categoryFilter = modelTabs[currentIndex].type
            }
        }

        CustomTextField {
            id: txtFilter
            Layout.fillWidth: true
            placeholderText: "Enter symbol..."
            onTextChanged: availableInsts.symbolFilter = txtFilter.text
        }

        RowLayout {
            Layout.fillWidth: true
            CustomText {
                text: internal.advancedOpen ? "▾ Advanced" : "▸ Advanced"
                color: "#8bc34a"
                font.pixelSize: Theme.fontSizeBody
                MouseArea {
                    anchors.fill: parent
                    onClicked: internal.advancedOpen = !internal.advancedOpen
                }
            }
            Item { Layout.fillWidth: true }
        }

        ColumnLayout {
            Layout.fillWidth: true
            visible: internal.advancedOpen

            RowLayout {
                Layout.fillWidth: true
                CustomText {
                    text: "Category:"
                    font.pixelSize: Theme.fontSizeSmall
                }
                Item { Layout.fillWidth: true }
                CustomText {
                    text: internal.selectedCategory
                    font.pixelSize: Theme.fontSizeSmall
                }
            }

            RowLayout {
                Layout.fillWidth: true
                CustomText {
                    text: "Symbol:"
                    font.pixelSize: Theme.fontSizeSmall
                }
                Item { Layout.fillWidth: true }
                CustomText {
                    text: internal.selectedSymbol
                    font.pixelSize: Theme.fontSizeSmall
                }
            }

            RowLayout {
                Layout.fillWidth: true
                CustomText {
                    text: "Base coin:"
                    font.pixelSize: Theme.fontSizeSmall
                }
                Item { Layout.fillWidth: true }
                CustomText {
                    text: internal.selectedBaseCoin
                    font.pixelSize: Theme.fontSizeSmall
                }
            }

            RowLayout {
                Layout.fillWidth: true
                CustomText {
                    text: "Quote coin:"
                    font.pixelSize: Theme.fontSizeSmall
                }
                Item { Layout.fillWidth: true }
                CustomText {
                    text: internal.selectedQuoteCoin
                    font.pixelSize: Theme.fontSizeSmall
                }
            }

            RowLayout {
                Layout.fillWidth: true
                CustomText {
                    text: "Status:"
                    font.pixelSize: Theme.fontSizeSmall
                }
                Item { Layout.fillWidth: true }
                CustomText {
                    text: internal.selectedStatus
                    font.pixelSize: Theme.fontSizeSmall
                }
            }
        }
        // -----------------------------------------------

        // --- Контент ---
        SplitView {
            id: mainLayout
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: Theme.margins
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

            // --- Левая колонка: доступные инструменты ---
            ListInsts {
                id: availableInsts
                SplitView.fillHeight: true
                SplitView.preferredWidth: mainLayout.width * 0.5
                SplitView.minimumWidth: 100

                listInsts: AppCore.marketState.availableInsts
                selectedId: internal.selectedInstrument.id

                onClickedOnInstrument: function(inst) {
                    internal.selectInstrument(inst)
                }
                onDoubleClickedOnInstrument: function(inst) {
                    internal.selectInstrument(inst)
                    internal.confirmOpen(inst)
                }
            }

            // --- Правая колонка: открытые инструменты ---
            ListInsts {
                id: openedInsts
                SplitView.fillHeight: true
                SplitView.preferredWidth: mainLayout.width * 0.5
                SplitView.minimumWidth: 100

                listInsts: AppCore.marketState.openedInsts
                selectedId: internal.selectedInstrument.id

                onClickedOnInstrument: function(inst) {
                    internal.selectInstrument(inst)
                }
                onDoubleClickedOnInstrument: function(inst) {
                    internal.selectInstrument(inst)
                    internal.confirmSelection(inst)
                }
            }
        }
        // -----------------------------------------------

        // --- Кнопки для выбора ---
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.margins

            CustomButton {
                text: "Cancel"
                implicitHeight: 30
                onClicked: root.close()
            }

            Item { Layout.fillWidth: true }

            CustomButton {
                text: "Open"
                enabled: internal.instrumentSelected
                implicitHeight: 30
                onClicked: internal.confirmOpen(internal.selectedInstrument)
            }

            CustomButton {
                text: "Close"
                enabled: internal.instrumentSelected
                implicitHeight: 30
                onClicked: internal.confirmClose(internal.selectedInstrument)
            }

            CustomButton {
                text: "Select"
                enabled: internal.instrumentSelected
                implicitHeight: 30
                onClicked: internal.confirmSelection(internal.selectedInstrument)
            }
        }
        // -----------------------------------------------
    }

} // root
