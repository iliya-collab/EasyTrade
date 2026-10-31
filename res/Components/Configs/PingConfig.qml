pragma Singleton
import QtQuick 2.15
import Theme 1.0

QtObject {
    // ===== Геометрия =====
    property real   size:             30
    property real   thickness:        2      // толщина дуги
    property real   startAngle:       -90    // старт с 12 часов

    // ===== Шкала =====
    property int    defaultMaxPing:   300    // значение, соответствующее полной окружности
    property int    goodMax:          50     // до этого значения — «хороший» пинг
    property int    warnMax:          150    // до этого значения — «средний», выше — «плохой»

    // ===== Цвета =====
    property color  goodColor:        "#2ecc71"
    property color  warnColor:        "#f1c40f"
    property color  badColor:         "#e74c3c"
    property color  trackColor:       Theme.isDark ? "#bdc3c7" : "#d0d4d8"   // фоновая дуга
    property color  idleTextColor:    "#7f8c8d"                              // нет данных

    // ===== Шрифт и тексты =====
    property int    fontSize:         Theme.fontSizeSmall
    property string emptyText:        "-"

    function colorFor(ping) {
        if (ping <= goodMax) return goodColor
        if (ping <= warnMax) return warnColor
        return badColor
    }
}
