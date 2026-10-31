pragma Singleton
import QtQuick 2.15
import Theme 1.0

QtObject {
    // ===== Размеры виджета =====
    property int    implicitWidth:        700
    property int    implicitHeight:       180
    property int    margins:              20
    property int    spacing:              12
    property int    titleSpacerHeight:    8      // пустой отступ под заголовком
    property int    scaleHeight:          70     // зона шкалы
    property int    axisHeight:           36     // зона оси X

    // ===== Трек шкалы =====
    property real   trackTop:             34     // верх трека внутри зоны шкалы
    property real   trackHeight:          14
    property real   padLeft:              40
    property real   padRight:             40
    property int    layerSamples:         4      // сглаживание Shape

    // ===== Маркеры =====
    property real   markerRadius:         6      // Low / High
    property real   currentRadius:        7      // точка Current
    property real   markerBorderWidth:    2
    property real   currentLineWidth:     2
    property real   currentLineExtent:    14     // выступ вертикали Current за трек

    // ===== Стрелка направления =====
    property real   arrowOffset:          12     // расстояние от Current до основания
    property real   arrowLength:          8
    property real   arrowHalfHeight:      5

    // ===== Ось X =====
    property int    axisTicks:            10
    property real   axisDivider:          1000
    property int    axisDecimals:         1
    property string axisSuffix:           "k"
    property real   axisPadLeft:          40
    property real   axisPadRight:         40

    // ===== Шрифты =====
    property int    titleFontSize:        18
    property int    labelFontSize:        12
    property int    axisFontSize:         11

    // ===== Цвета (зависят от темы) =====
    property color  markerBorderColor:    Theme.isDark ? "#1e222b" : "#f3f3f3"   // кольцо вокруг маркеров
    property color  trackColor:           "#5b8def"
    property color  lowColor:             Theme.redColor
    property color  highColor:            Theme.greenColor
    property color  neutralColor:         Theme.textSecondary                   // направление неизвестно
    property color  textColor:            Theme.textColor
    property color  titleColor:           Theme.textPrimary
    property color  axisColor:            Theme.isDark ? "#8b91a1" : "#6b7180"

    // ===== Тексты =====
    property string lowLabel:             "24h Low: "
    property string currentLabel:         "Current: "
    property string highLabel:            "24h High: "

    // ===== Значения по умолчанию, пока нет данных тикера =====
    property string fallbackSymbol:       "Unknown"
    property real   fallbackHigh:         1000
    property real   fallbackCurrentRatio: 0.25   // доля от high

    // ===== Форматирование =====
    // priceDecimals < 0 — выводить число как есть (прежнее поведение)
    property int    priceDecimals:        -1

    function formatPrice(v) {
        return priceDecimals < 0 ? String(v) : Number(v).toFixed(priceDecimals)
    }

    function formatAxis(v) {
        return (v / axisDivider).toFixed(axisDecimals) + axisSuffix
    }
}
