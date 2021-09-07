#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDateTime>
#include <QVersionNumber>
#include <QWindow>

void onLoadComplete(QWindow *window) {
    QVersionNumber appVersion(VERSION_MAJOR, VERSION_MINOR, VERSION_MIC);
    QString versionString = appVersion.toString();
#ifdef QT_DEBUG
    QDateTime dateTime = QDateTime::currentDateTime();
    versionString += "-debug." + QString::number(VERSION_DEBUG) + "+" + dateTime.toString("yyyyMMddhhmmss");
#endif
    QString windowTitle = "Kijang (" + versionString + ")";
    window->setTitle(windowTitle);
    window->showMaximized();
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
        QWindow *window = qobject_cast<QWindow*>(obj);
        if (!window) QCoreApplication::exit(-1);
        onLoadComplete(window);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
