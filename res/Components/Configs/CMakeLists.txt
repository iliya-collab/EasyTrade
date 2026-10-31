set(CONFIG_QML
    KlineChartConfig.qml
    OrderbookConfig.qml
    TickerConfig.qml
    SentimentConfig.qml
    OrdersConfig.qml
    PingConfig.qml
    TradesConfig.qml
    TradeHistoryConfig.qml
)

set_source_files_properties(${CONFIG_QML}
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(ConfigsComponents
    URI Components.Configs
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
    OUTPUT_DIRECTORY "${QML_MODULES}/Components/Configs"
    QML_FILES ${CONFIG_QML}
)