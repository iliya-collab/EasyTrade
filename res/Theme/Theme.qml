pragma Singleton
import QtQuick
import QtCore

QtObject {
    id: root

    // ===== Выбор и определение темы =====
    // mode: "system" (следовать ОС) | "dark" | "light"
    property alias mode: store.mode

    readonly property bool systemIsDark: Qt.styleHints.colorScheme !== Qt.ColorScheme.Light

    // Итоговая тема
    readonly property bool isDark:          mode === "dark" || (mode !== "light" && systemIsDark)
    readonly property string effectiveName: isDark ? "dark" : "light"

    property Settings _store: Settings {
        id: store
        category: "Appearance"
        property string mode: "system"
    }

    readonly property color windowColor:            isDark ? "#1f1f1f" : "#f3f3f3"
    readonly property color menuBarColor:           isDark ? "#1f1f1f" : "#f3f3f3"
    readonly property color toolBarColor:           isDark ? "#1f1f1f" : "#f3f3f3"
    readonly property color buttonColor:            isDark ? "#1c1c1c" : "#e6e6e6"
    readonly property color hoverColor:             isDark ? "#a6a6a6" : "#d6d6d6"
    readonly property color pressColor:             isDark ? "#a6a6a6" : "#c4c4c4"
    readonly property color selectColor:            isDark ? "#a6a6a6" : "#c4c4c4"
    readonly property color disabledButtonColor:    isDark ? "#3a3a3a" : "#e0e0e0"
    readonly property color disabledBorderColor:    isDark ? "#555555" : "#c8c8c8"
    readonly property color disabledTextColor:      isDark ? "#888888" : "#9a9a9a"
    readonly property color focusColor:             isDark ? "#4caf50" : "#388e3c"
    readonly property color greenColor:             isDark ? "#00ff66" : "#00a152"
    readonly property color redColor:               isDark ? "#ff4444" : "#d32f2f"

    // Switch
    readonly property int   switchWidth:            40
    readonly property int   switchHeight:           22
    readonly property color switchBackgroundColor:  isDark ? "#3a3a3a" : "#bdbdbd"
    readonly property color switchCheckedColor:     isDark ? "#4caf50" : "#4caf50"
    readonly property color switchHandleColor:      isDark ? "#f0f0f0" : "#ffffff"

    // CheckBox
    readonly property color indicatorColor: isDark ? "#ffffff" : "#222222"
    readonly property int widthBox:         20

    // TextField
    readonly property color textFieldColor:         isDark ? "#1c1c1c" : "#ffffff"
    readonly property color hoverTextFieldColor:    isDark ? "#d4d4d4" : "#e0e0e0"
    readonly property color placeholderColor:       isDark ? "#808080" : "#8a8a8a"
    readonly property color selectionColor:         isDark ? "#4caf50" : "#4caf50"
    readonly property color selectedTextColor:      isDark ? "#ffffff" : "#ffffff"
    readonly property color disabledFieldColor:     isDark ? "#252525" : "#ececec"

    // ScrollBar
    readonly property color sliderColor:        isDark ? "#2e2e2e" : "#c8c8c8" // цвет бегунка
    readonly property color scrollBarColor:     "transparent" // цвет скролл-бара
    readonly property int scrollBarWidth:       5 // ширина скролл-бара
    readonly property int scrollBarHeight:      100 // высота скролл-бара
    readonly property double scrollBarMinSize:  0.05 // минимальный размер бегунка

    // Обшие параметры
    readonly property int padding:          10
    readonly property int margins:          10
    readonly property int spacing:          10
    readonly property int radius:           10
    readonly property color borderColor:    isDark ? "#adadad" : "#9e9e9e"
    readonly property color accentColor:    isDark ? "#adadad" : "#9e9e9e"
    readonly property int borderWidth:      1

    // Шрифт
    readonly property int fontSizeHeader:       20 // размер заголовков
    readonly property int fontSizeBody:         14 // размер основного текста
    readonly property int fontSizeSmall:        11 // размер маленького текста
    readonly property string fontFamily:        "Segoe UI" // шрифт
    readonly property color textColor:          isDark ? "#ffffff" : "#1a1a1a" // цвет текста
    readonly property color  textPrimary:       isDark ? "#ffffff" : "#1a1a1a"
    readonly property color  textSecondary:     isDark ? "#aaaaaa" : "#666666"
    readonly property color selectTextColor:    isDark ? "#ffffff" : "#1a1a1a"
}
