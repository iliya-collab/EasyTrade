pragma Singleton
import QtQuick 2.15
import Theme 1.0

QtObject {
    // ===== Колонки (доли ширины списка) =====
    property real   priceField:       0.25
    property real   volumeField:      0.25
    property real   turnoverField:    0.25
    property real   timeField:        0.25

    // ===== Геометрия =====
    property int    margins:          Theme.margins
    property int    rowPadding:       Theme.padding

    // ===== Цвета =====
    property color  buyColor:         Theme.greenColor
    property color  sellColor:        Theme.redColor
    property color  textColor:        Theme.textColor   // объём, оборот, цена без стороны
    property color  timeColor:        Theme.textSecondary

    // ===== Шрифт =====
    property string fontFamily:       "Monospace"
    property int    fontSize:         Theme.fontSizeSmall

    // ===== Форматирование =====
    property string emptyText:        "-"
    property int    priceDecimals:    2
    property int    amountDecimals:   2
    property string timeFormat:       "hh:mm:ss"

    function sideColor(side) {
        if (!side)
            return textColor
        return side === "Buy" ? buyColor : sellColor
    }

    function formatPrice(v) {
        return v ? Number(v).toFixed(priceDecimals) : emptyText
    }

    // Объём / оборот: 1.23K, 4.56M
    function formatAmount(v) {
        if (!v)
            return emptyText
        v = Number(v)
        if (v >= 1000000) return (v / 1000000).toFixed(amountDecimals) + "M"
        if (v >= 1000)    return (v / 1000).toFixed(amountDecimals) + "K"
        return v.toFixed(amountDecimals)
    }

    // Число — отметка времени в мс, строка выводится как есть
    function formatTime(t) {
        if (!t)
            return emptyText
        if (typeof t === "number")
            return Qt.formatDateTime(new Date(t), timeFormat)
        return t
    }
}
