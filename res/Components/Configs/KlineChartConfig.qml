pragma Singleton
import QtQuick 2.15
import Theme 1.0

QtObject {
    // ===== Цвета =====
    property color background:        Theme.isDark ? "#1e1e1e" : "#ffffff"
    property color border:            Theme.isDark ? "#3a3a3a" : "#d0d0d0"
    property color separator:         Theme.isDark ? "#3a3a3a" : "#d0d0d0"
    property color gridLine:          Theme.isDark ? "#2d2d2d" : "#ececec"
    property color gridLineVertical:  Theme.isDark ? "#252525" : "#f2f2f2"
    property color axisText:          Theme.isDark ? "#8a8a8a" : "#666666"
    property color axisLabelText:     Theme.isDark ? "#ffffff" : "#1a1a1a"

    // ===== Свечи =====
    property color bullConfirmed:     Theme.isDark ? "#00C853" : "#00A152"
    property color bearConfirmed:     Theme.isDark ? "#FF5252" : "#E53935"
    property color bullUnconfirmed:   "#66BB6A"
    property color bearUnconfirmed:   "#EF5350"

    // ===== Кросс-курсор =====
    property color crosshairLine:     Theme.isDark ? Qt.rgba(1, 1, 1, 0.3) : Qt.rgba(0, 0, 0, 0.3)
    property color crosshairDot:      Theme.isDark ? Qt.rgba(1, 1, 1, 0.5) : Qt.rgba(0, 0, 0, 0.5)
    property color tooltipBg:         Theme.isDark ? Qt.rgba(30/255, 30/255, 30/255, 0.85) : Qt.rgba(1, 1, 1, 0.92)
    property color tooltipBorder:     Theme.isDark ? Qt.rgba(1, 1, 1, 0.2) : Qt.rgba(0, 0, 0, 0.2)
    property color tooltipText:       Theme.isDark ? "#ffffff" : "#1a1a1a"

    // ===== Шрифты =====
    property string fontFamily:       "sans-serif"
    property string infoFontFamily:   "monospace"
    property int    axisFontSize:     10
    property int    infoFontSize:     11
    property int    tooltipFontSize:  10

    // ===== Геометрия =====
    property int candleWidthDefault:  20
    property int candleWidthMin:      2
    property int candleWidthMax:      80
    property int candleSpacing:       6
    property int zoomStep:            2
    property int tooltipRadius:       3
    property int priceAxisWidth:      60
    property int timeAxisHeight:      40

    // ===== Поведение =====
    property real volumeChartHeightRatio: 0.25
    property int  autoScrollDelayMs:      3000
    property int  scrollAnimationMs:      200
    property int  priceGridTargetLines:   5
    property int  volumeGridTargetLines:  3
    property real pricePaddingRatio:      0.15
    property real volumePaddingRatio:     1.5

    // ===== Оси времени =====
    property string timeFormat:        "HH:mm"
    property string cursorTimeFormat:  "dd.MM.yyyy HH:mm"

    // Шаги подписей времени в зависимости от ширины свечи
    property int timeLabelStepTiny:    20
    property int timeLabelStepSmall:   10
    property int timeLabelStepNormal:  5
    property int timeLabelStepLarge:   2
    property int timeLabelStepHuge:    1

    // ===== Форматирование чисел =====
    property int priceDecimals:        2

    function formatPrice(value) {
        return (typeof value === "number" && isFinite(value))
                ? value.toFixed(priceDecimals) : "-"
    }

    function formatVolume(value) {
        if (typeof value !== "number" || !isFinite(value))
            return "-"
        if (value >= 1e6) return (value / 1e6).toFixed(2) + "M"
        if (value >= 1e3) return (value / 1e3).toFixed(2) + "K"
        return value.toFixed(2)
    }
}