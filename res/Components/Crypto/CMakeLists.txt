qt_add_qml_module(CryptoComponents
    URI Components.Crypto
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
    OUTPUT_DIRECTORY "${QML_MODULES}/Components/Crypto"
    QML_FILES
        SelecterTradeWidget.qml
        OrderbookWidget.qml
        TickerWidget.qml
        KlineWidget.qml
        IndicatorMarketSentiment.qml
        KlineChart.qml
        PingIndicator.qml
        PublicTradesWidget.qml
        OrdersPanel.qml
        OrderbookView.qml
)
