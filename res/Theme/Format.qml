pragma Singleton
import QtQuick

QtObject {

    readonly property real  riskWarnLevel:   0.5
    readonly property real  riskDangerLevel: 0.8

    // ------------------------------------------------------------------
    //  Хелперы форматирования
    // ------------------------------------------------------------------
    function num(s)
    {
        const n = Number(s)
        return isNaN(n) ? 0 : n
    }

    function fmt(s, d)
    {
        return num(s).toLocaleString(Qt.locale(), 'f', d === undefined ? 4 : d)
    }

    function pct(s)
    {
        return (num(s) * 100).toFixed(2) + " %"
    }

    function pnlColor(s)
    {
        const n = num(s)
        if (n > 0)
            return Theme.greenColor
        if (n < 0)
            return Theme.redColor
        return Theme.textColor
    }

    function riskColor(s)
    {
        const n = num(s)
        if (n >= riskDangerLevel)
            return Theme.redColor
        if (n >= riskWarnLevel)
            return Theme.warningColor
        return Theme.textColor
    }

}
