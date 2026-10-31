pragma Singleton
import QtQuick 2.15
import Theme 1.0

// Общие настройки таблиц ордеров (OpenedOrdersWidget, HistoricalOrdersWidget)
QtObject {
    // ===== Геометрия =====
    property int    margins:               Theme.margins
    property int    rowHeight:             20
    property int    rowHorizontalMargin:   5
    property int    rowSpacing:            4
    property int    radius:                Theme.radius
    property int    borderWidth:           Theme.borderWidth

    // ===== Раскрывающийся блок =====
    property int    detailsPadding:        10
    property int    detailsExtraHeight:    4      // добавка к высоте раскрытого блока
    property int    detailsInnerMargin:    2
    property int    detailsSpacing:        8

    // ===== Анимации (мс) =====
    property int    colorAnimMs:           100
    property int    heightAnimMs:          150
    property int    opacityAnimMs:         100

    // ===== Цвета (зависят от темы) =====
    property color  textSecondary:         Theme.isDark ? "#aaaaaa" : "#666666"   // ячейки и заголовки
    property color  sortArrowColor:        Theme.isDark ? "#8bc34a" : "#558b2f"
    property color  buyColor:              Theme.greenColor
    property color  sellColor:             Theme.redColor
    property color  borderHoverColor:      Theme.borderColor
    property color  borderExpandedColor:   Theme.selectionColor
    property color  detailsBackground:     Theme.isDark ? "#1a1a1a" : "#ececec"
    property color  detailsTitleColor:     Theme.selectionColor

    // ===== Шрифты =====
    property string cellFontFamily:        "monospace"
    property int    cellFontSize:          Theme.fontSizeSmall
    property int    headerFontSize:        Theme.fontSizeSmall
    property int    arrowFontSize:         Theme.fontSizeBody
    property int    detailsFontSize:       Theme.fontSizeSmall
    property int    buttonFontSize:        Theme.fontSizeSmall

    // ===== Тексты =====
    property string sortDescText:          "↓"
    property string sortAscText:           "↑"
    property string cancelAllText:         "Cancel All"
    property string amendText:             "Amend"
    property string cancelText:            "Cancel"
    property string actionsTitle:          "Actions with order"
    property string executionsTitlePrefix: "Executions for order "

    // ===== Колонки: открытые ордера =====
    property var openedColumns: [
        { title: "Category",   width: 90  },
        { title: "Symbol",     width: 80  },
        { title: "Type",       width: 70  },
        { title: "Side",       width: 60  },
        { title: "Leaves/Qty", width: 90  },
        { title: "Price",      width: 80  },
        { title: "Order ID",   width: 120 },
        { title: "Created",    width: 100 }
    ]

    // ===== Колонки: история ордеров =====
    property var historicalColumns: [
        { title: "Category",   width: 90  },
        { title: "Symbol",     width: 80  },
        { title: "Type",       width: 70  },
        { title: "Side",       width: 60  },
        { title: "Leaves/Qty", width: 90  },
        { title: "Price",      width: 90  },
        { title: "Order ID",   width: 140 },
        { title: "Status",     width: 120 },
        { title: "Created",    width: 140 }
    ]

    // ===== Колонки: исполнения внутри карточки истории =====
    property var executionColumns: [
        { title: "Exec ID",    width: 140 },
        { title: "Exec Time",  width: 160 },
        { title: "Exec Qty",   width: 90  },
        { title: "Exec Price", width: 90  },
        { title: "Exec Fee",   width: 110 }
    ]

    // ===== Форматирование =====
    property string timeFormat:            "dd.MM.yyyy HH:mm:ss"

    function formatTime(ms) {
        const value = Number(ms)
        if (!value)
            return ""
        return Qt.formatDateTime(new Date(value), timeFormat)
    }

    function sideColor(side) {
        return side === "Buy" ? buyColor : (side === "Sell" ? sellColor : textSecondary)
    }
}
