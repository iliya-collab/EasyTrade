pragma Singleton
import QtQuick 2.15
import Theme 1.0

QtObject {
    // ===== Геометрия =====
    property int    rowHeight:            20
    property int    headerHeight:         30
    property int    margins:              10     // внешние отступы заголовка и списков
    property int    listsTopMargin:       5
    property int    rowHorizontalMargin:  5      // отступы внутри строки
    property int    rowSpacing:           4
    property real   columnRatio:          0.30   // доля ширины колонки (оборот / объём)
    property int    separatorWidth:       1

    // ===== Цвета: общие =====
    property color  separatorColor:       Theme.isDark ? "#4a4a6a" : "#c8c8d8"
    property color  textPrimary:          Theme.textPrimary
    property color  textSecondary:        Theme.textSecondary

    // ===== Цвета: BIDS =====
    property color  bidColor:             Theme.greenColor   // текст цены, рамка hover
    property color  bidFill:              "#4caf50"          // заливка объёмной полосы
    property color  bidHoverBg:           "#8bc34a"          // фон подсвеченных строк
    property color  headerBidColor:       bidColor

    // ===== Цвета: ASKS =====
    property color  askColor:             Theme.redColor
    property color  askFill:              "#f44336"
    property color  askHoverBg:           "#f44336"
    property color  headerAskColor:       Theme.isDark ? "#ff4444" : "#d32f2f"

    // ===== Прозрачность объёмных полос =====
    property real   barAlphaFaint:        0.05
    property real   barAlphaFaintHover:   0.10
    property real   barAlphaStrong:       0.30
    property real   barAlphaStrongHover:  0.80
    property real   hoverRowAlpha:        0.08

    // ===== Шрифт строк и tooltip =====
    property string fontFamily:           "monospace"
    property int    fontSize:             11

    // ===== Tooltip =====
    property int    tooltipWidth:         220
    property int    tooltipPadding:       8
    property int    tooltipGap:           14     // зазор между курсором и tooltip
    property int    tooltipEdgeMargin:    5      // минимальный отступ от краёв виджета
    property int    tooltipRadius:        2
    property color  tooltipBg:            Theme.isDark ? Qt.rgba(30 / 255, 30 / 255, 40 / 255, 0.92)
                                                       : Qt.rgba(1, 1, 1, 0.96)

    // ===== Форматирование =====
    property int    priceDecimals:        2
    property int    numberDecimals:       2

    function formatPrice(v) {
        return Number(v).toFixed(priceDecimals)
    }

    function formatNumber(v) {
        v = Number(v)
        if (v >= 1000000) return (v / 1000000).toFixed(numberDecimals) + "M"
        if (v >= 1000)    return (v / 1000).toFixed(numberDecimals) + "K"
        return v.toFixed(numberDecimals)
    }

    // Цвет c с заданной прозрачностью (для градиентов и подсветки)
    function withAlpha(c, a) {
        return Qt.rgba(c.r, c.g, c.b, a)
    }
}
