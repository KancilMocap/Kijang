#include "kijang.h"

Kijang::Kijang()
{
    qDebug() << "Application constructed";
}

Kijang::~Kijang()
{
    qDebug() << "Application deconstructed";
}

int Kijang::run(int argc, char **argv)
{

    // Set application info
    QCoreApplication::setApplicationName("Kijang");
    QCoreApplication::setOrganizationName("KancilMocap");
    QCoreApplication::setOrganizationDomain("kancil.moe");
    QVersionNumber appVersion(VERSION_MAJOR, VERSION_MINOR, VERSION_MIC);
    QString versionString = appVersion.toString();
    #ifdef QT_DEBUG
        QDateTime dateTime = QDateTime::currentDateTime();
        versionString += "-debug." + QString::number(VERSION_DEBUG) + "+" + dateTime.toString("yyyyMMddhhmmss");
    #endif
    QCoreApplication::setApplicationVersion(versionString);

    // Attach logger
    KijangLogger::attach();

    // Loads home page
    QGuiApplication app(argc, argv);
    m_engine = QSharedPointer<QQmlApplicationEngine>(new QQmlApplicationEngine());
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(m_engine.data(), &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    m_engine->load(url);

    return app.exec();
}

QString Kijang::getTypeQml(PageType type)
{
    switch (type) {
    case PageType::Home:
        return "res/pages/Home.qml";
    case PageType::Input:
        return "res/pages/Input.qml";
    case PageType::Settings:
        return "res/pages/Settings.qml";
    }
}
