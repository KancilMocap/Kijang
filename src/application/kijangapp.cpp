#include "kijangapp.h"

Q_DECLARE_LOGGING_CATEGORY(application);
Q_LOGGING_CATEGORY(application,"application");

QString KijangApp::settingsFile = QCoreApplication::applicationDirPath() + "/settings.ini";

KijangApp::KijangApp(QObject *parent) : QObject(parent)
{
    qDebug(application) << "Application constructed, loading settings...";
    // TODO: Load settings
    // Don't use setters here as it may cause network and serial manager to be prematurely started
    m_ethernetEnabled = true;
    m_serialEnabled = false;
}

KijangApp::~KijangApp()
{
    qDebug(application) << "Application deconstructed, saving settings...";
    // TODO: Save settings
}

int KijangApp::run(int argc, char **argv)
{
    qRegisterMetaType<Kijang::KijangProtocol>();

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
    KijangLogger logger;
    m_engine->rootContext()->setContextProperty("kijangApp", this);
    m_engine->rootContext()->setContextProperty("kijangLogsUI", &logger);
    m_engine->rootContext()->setContextProperty("kijangInputManager", &m_inputManager);
    m_engine->rootContext()->setContextProperty("kijangNetworkManager", &m_networkManager);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(m_engine.data(), &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    m_engine->load(url);

    // Initialises servers if needed
    m_inputManager.start();
    // Serial manager and network manager would have been automatically started from QML

    return app.exec();
}

const QString &KijangApp::getSettingsFile()
{
    return settingsFile;
}

const KijangNetworkManager &KijangApp::networkManager() const
{
    return m_networkManager;
}

bool KijangApp::ethernetEnabled() const
{
    return m_ethernetEnabled;
}

void KijangApp::setEthernetEnabled(bool newEthernetEnabled)
{
    if (m_ethernetEnabled != newEthernetEnabled) {
        m_ethernetEnabled = newEthernetEnabled;
        emit ethernetEnabledChanged();
        if (newEthernetEnabled) {
            m_networkManager.start();
        } else {
            m_networkManager.stop();
        }
    }
}

bool KijangApp::serialEnabled() const
{
    return m_serialEnabled;
}

void KijangApp::setSerialEnabled(bool newSerialEnabled)
{
    m_serialEnabled = newSerialEnabled;
}

const KijangPluginManager &KijangApp::pluginManager() const
{
    return m_pluginManager;
}

const KijangInputManager &KijangApp::inputManager() const
{
    return m_inputManager;
}
