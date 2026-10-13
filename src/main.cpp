#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>

int main(int argc, char *argv[]) {
    //qputenv("QT_QPA_PLATFORM", "xcb");
    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");

    QGuiApplication app(argc, argv);

    app.setWindowIcon(QIcon("://icons/icon_app.png"));

    QQmlApplicationEngine qmlEngine;

    // Пути поиска
    qDebug() << "Import paths:" << qmlEngine.importPathList();

    QObject::connect(&qmlEngine, &QQmlApplicationEngine::objectCreationFailed, &app, []() {
        QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    qmlEngine.loadFromModule("Application.UI", "Main");

    return app.exec();
}
