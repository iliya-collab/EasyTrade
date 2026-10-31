pragma Singleton
import QtQuick 2.15
import Theme 1.0

// Таблица истории исполнений. Общий стиль таблиц берётся из OrdersConfig,
// чтобы обе таблицы выглядели одинаково; любое значение можно переопределить здесь.
QtObject {
    // ===== Геометрия =====
    property int    margins:              OrdersConfig.margins
    property int    rowHeight:            OrdersConfig.rowHeight
    property int    rowHorizontalMargin:  OrdersConfig.rowHorizontalMargin
    property int    rowSpacing:           OrdersConfig.rowSpacing

    // ===== Цвета =====
    property color  textSecondary:        OrdersConfig.textSecondary
    property color  buyColor:             OrdersConfig.buyColor
    property color  sellColor:            OrdersConfig.sellColor

    // ===== Шрифты =====
    property string cellFontFamily:       OrdersConfig.cellFontFamily
    property int    cellFontSize:         OrdersConfig.cellFontSize
    property int    headerFontSize:       OrdersConfig.headerFontSize

    // ===== Колонки =====
    property var columns: [
        { title: "Market",      width: 80  },
        { title: "Symbol",      width: 80  },
        { title: "Side",        width: 60  },
        { title: "Exec Value",  width: 100 },
        { title: "Exec Price",  width: 90  },
        { title: "Exec Qty",    width: 90  },
        { title: "Exec Type",   width: 80  },
        { title: "Fee",         width: 90  },
        { title: "Exec ID",     width: 120 },
        { title: "Time",        width: 140 }
    ]

    // ===== Форматирование =====
    property string timeFormat:           "dd.MM.yyyy HH:mm:ss"

    function formatTime(ms) {
        const value = Number(ms)
        if (!value)
            return ""
        return Qt.formatDateTime(new Date(value), timeFormat)
    }

    function formatFee(fee, currency) {
        return fee + " " + currency
    }

    function sideColor(side) {
        return side === "Buy" ? buyColor : (side === "Sell" ? sellColor : textSecondary)
    }
}
