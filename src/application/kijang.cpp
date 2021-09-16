#include "kijang.h"

Q_DECLARE_LOGGING_CATEGORY(application);
Q_LOGGING_CATEGORY(application,"application");

QString Kijang::settingsFile = QCoreApplication::applicationDirPath() + "/settings.ini";

Kijang::Kijang(QObject *parent) : QObject(parent)
{
    qDebug(application) << "Application constructed, loading settings...";
    QSettings settings;
    settings.beginGroup("output");
    setEthernetEnabled(settings.value("enable_ethernet", true).toBool());
    setSerialEnabled(settings.value("enable_serial", true).toBool());
    settings.endGroup();
}

Kijang::~Kijang()
{
    qDebug(application) << "Application deconstructed, saving settings...";
    QSettings settings;
    settings.beginGroup("output");
    settings.setValue("enable_ethernet", m_ethernetEnabled);
    settings.setValue("enable_serial", m_serialEnabled);
    settings.endGroup();
    settings.sync();
}

int Kijang::run(int argc, char **argv)
{
    qRegisterMetaType<KijangProtocol>();

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
    if (m_serialEnabled) {
        // TODO: Enable serial communication
    }
    if (m_ethernetEnabled) {
        m_networkManager.start();
    }

    return app.exec();
}

const QString &Kijang::getSettingsFile()
{
    return settingsFile;
}

const KijangNetworkManager &Kijang::networkManager() const
{
    return m_networkManager;
}

bool Kijang::ethernetEnabled() const
{
    return m_ethernetEnabled;
}

void Kijang::setEthernetEnabled(bool newEthernetEnabled)
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

bool Kijang::serialEnabled() const
{
    return m_serialEnabled;
}

void Kijang::setSerialEnabled(bool newSerialEnabled)
{
    m_serialEnabled = newSerialEnabled;
}

const KijangInputManager &Kijang::inputManager() const
{
    return m_inputManager;
}
