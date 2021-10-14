#include "kijangnetworkmanager.h"

KijangNetworkManager::KijangNetworkManager(QObject *parent) : QObject(parent)
{
    commServer.setType(KijangTcpServer::ServerType::Communication);
    statusServer.setType(KijangTcpServer::ServerType::Status);
    QSettings settings;
    settings.beginGroup("network");
    bool ok;
    setCommServerPort(settings.value("comm_port", 4318).toUInt(&ok));
    if (!ok) m_commServerPort = 4318;
    setStatusServerPort(settings.value("status_port", 4319).toUInt(&ok));
    if (!ok) m_statusServerPort = 4319;
    settings.endGroup();
}

KijangNetworkManager::~KijangNetworkManager()
{
    QSettings settings;
    settings.beginGroup("network");
    settings.setValue("comm_port", m_commServerPort);
    settings.setValue("status_port", m_statusServerPort);
    settings.endGroup();
    settings.sync();
}

void KijangNetworkManager::start()
{
    if (commServer.start(m_commServerPort)) {
        setCommServerError(false);
    } else {
        setCommServerError(true);
        setCommServerErrorString(commServer.errorString());
    }
    if (statusServer.start(m_statusServerPort)) {
        setStatusServerError(false);
    } else {
        setStatusServerError(true);
        setStatusServerErrorString(statusServer.errorString());
    }
}

void KijangNetworkManager::stop()
{
    if (commServer.started()) commServer.quit();
    if (statusServer.started()) statusServer.quit();
}

quint16 KijangNetworkManager::commServerPort() const
{
    return m_commServerPort;
}

void KijangNetworkManager::setCommServerPort(quint16 newCommServerPort)
{
    if (newCommServerPort != m_commServerPort) {
        m_commServerPort = newCommServerPort;
        emit commServerPortChanged();

        if (commServer.started()) commServer.quit();
        if (commServer.start(newCommServerPort)) {
            setCommServerError(false);
        } else {
            setCommServerError(true);
            setCommServerErrorString(commServer.errorString());
        }
    }
}

quint16 KijangNetworkManager::statusServerPort() const
{
    return m_statusServerPort;
}

void KijangNetworkManager::setStatusServerPort(quint16 newStatusServerPort)
{
    if (newStatusServerPort != m_statusServerPort) {
        m_statusServerPort = newStatusServerPort;
        emit statusServerPortChanged();

        if (statusServer.started()) statusServer.quit();
        if (statusServer.start(newStatusServerPort)) {
            setStatusServerError(false);
        } else {
            setStatusServerError(true);
            setStatusServerErrorString(statusServer.errorString());
        }
    }
}

bool KijangNetworkManager::commServerError() const
{
    return m_commServerError;
}

void KijangNetworkManager::setCommServerError(bool newCommServerError)
{
    if (m_commServerError == newCommServerError)
        return;
    m_commServerError = newCommServerError;
    emit commServerErrorChanged();
}

bool KijangNetworkManager::statusServerError() const
{
    return m_statusServerError;
}

void KijangNetworkManager::setStatusServerError(bool newStatusServerError)
{
    if (m_statusServerError == newStatusServerError)
        return;
    m_statusServerError = newStatusServerError;
    emit statusServerErrorChanged();
}

const QString &KijangNetworkManager::commServerErrorString() const
{
    return m_commServerErrorString;
}

void KijangNetworkManager::setCommServerErrorString(const QString &newCommServerErrorString)
{
    if (m_commServerErrorString == newCommServerErrorString)
        return;
    m_commServerErrorString = newCommServerErrorString;
    emit commServerErrorStringChanged();
}

const QString &KijangNetworkManager::statusServerErrorString() const
{
    return m_statusServerErrorString;
}

void KijangNetworkManager::setStatusServerErrorString(const QString &newStatusServerErrorString)
{
    if (m_statusServerErrorString == newStatusServerErrorString)
        return;
    m_statusServerErrorString = newStatusServerErrorString;
    emit statusServerErrorStringChanged();
}

void KijangNetworkManager::statusAutodetect()
{
    statusServer.start(0, true);
}

void KijangNetworkManager::commAutodetect()
{
    commServer.start(0, true);
}

void KijangNetworkManager::addModule(KijangModuleHandler *module)
{
    quint16 moduleID = module->module();
    QLoggingCategory network("network");
    if (moduleHandlerList.contains(moduleID)) {
        qWarning(network) << "Could not add module" << moduleID << "as module already exists";
    } else if (moduleID | 32768) {
        qWarning(network) << "Attempting to add client module to server for module" << moduleID;
    } else if (moduleID >= 32765 && moduleID <= 32767 ) {
        qWarning(network) << "Attempting to add protected modules";
    } else {
        moduleHandlerList.insert(moduleID, module);
    }
}

void KijangNetworkManager::removeModule(quint16 module)
{
    QLoggingCategory network("network");
    if (module | 32768) {
        qWarning(network) << "Attempting to remove client module from server";
    } else if (moduleHandlerList.contains(module)) {
        moduleHandlerList.remove(module);
    } else if (module >= 32765 && module == 32767) {
        qWarning(network) << "Attempting to remove protected modules";
    } else {
        qInfo(network) << "Module" << module << "does not exist";
    }
}
