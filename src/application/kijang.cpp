#include "kijang.h"

Q_DECLARE_LOGGING_CATEGORY(application);
Q_LOGGING_CATEGORY(application,"application");

QString Kijang::settingsFile = QCoreApplication::applicationDirPath() + "/settings.ini";

Kijang::Kijang(QObject *parent) : QObject(parent)
{
    qDebug(application) << "Application constructed";
}

Kijang::~Kijang()
{
    qDebug(application) << "Application deconstructed";
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
    qInfo(application) << "Starting" << QCoreApplication::applicationName();
    qInfo(application) << "Current version:" << versionString;

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

    // Initialises servers if needed
    QSettings settings;
    settings.beginGroup("output");
    m_inputManager.start();
    if (settings.value("enable_serial", false).toBool()) {
        // TODO: Enable serial communication
    }
    if (settings.value("enable_ethernet", false).toBool()) {
        // TODO: Start network manager
    }
    settings.endGroup();

    return app.exec();
}

const QString &Kijang::getSettingsFile()
{
    return settingsFile;
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
    return nullptr;
}

const KijangNetworkManager &Kijang::networkManager() const
{
    return m_networkManager;
}

const KijangInputManager &Kijang::inputManager() const
{
    return m_inputManager;
}
