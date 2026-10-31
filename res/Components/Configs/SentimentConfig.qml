pragma Singleton
import QtQuick 2.15
import Theme 1.0

QtObject {
    // ===== Цвета =====
    property color  bidColor:         Theme.greenColor
    property color  askColor:         Theme.redColor
    property color  barTextColor:     Theme.textColor   // проценты на полосах
    property color  markerBackground: "transparent"

    // ===== Геометрия =====
    property int    markerPadding:      Theme.margins      // добавка к высоте текста маркера B/A
    property int    markerBorderWidth:  Theme.borderWidth
    property int    markerRadius:       Theme.radius
    property int    textMargin:         Theme.margins / 2  // отступ процентов от края полосы
    property int    sentimentHeight:    20

    // ===== Шрифт =====
    property int    fontSize:         Theme.fontSizeBody
    property string fontFamily:       Theme.fontFamily

    // ===== Тексты =====
    property string bidLabel:         "B"
    property string askLabel:         "A"

    // ===== Форматирование =====
    // percentDecimals < 0 — выводить число как есть (прежнее поведение)
    property int    percentDecimals:  -1

    function formatPercent(v) {
        return (percentDecimals < 0 ? String(v) : Number(v).toFixed(percentDecimals)) + "%"
    }
}
