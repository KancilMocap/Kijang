#include "kijangnetworkmanager.h"

QString KijangNetworkManager::settingsFile = QDir::currentPath() + QDir::separator() + "settings.ini";

KijangNetworkManager::KijangNetworkManager(QObject *parent) : QObject(parent)
{
    QSettings settings(settingsFile, QSettings::IniFormat);
    settings.beginGroup("network");
    QLoggingCategory network("network");
    qDebug(network) << "Network manager constructed";
    bool ok;
    m_commServerPort = settings.value("comm_port", 4318).toUInt(&ok);
    if (!ok) m_commServerPort = 4318;
    m_statusServerPort = settings.value("status_port", 4319).toUInt(&ok);
    if (!ok) m_statusServerPort = 4319;
    settings.endGroup();
}

KijangNetworkManager::~KijangNetworkManager()
{
    QSettings settings(settingsFile, QSettings::IniFormat);
    settings.beginGroup("network");
    settings.setValue("comm_port", m_commServerPort);
    settings.setValue("status_port", m_statusServerPort);
    settings.endGroup();
    settings.sync();
}

void KijangNetworkManager::start()
{
    resetCommServer();
    resetStatusServer();
    if (commServer->start(m_commServerPort)) {
        setCommServerError(false);
    } else {
        setCommServerError(true);
        setCommServerErrorString(commServer->errorString());
    }
    if (statusServer->start(m_statusServerPort)) {
        setStatusServerError(false);
    } else {
        setStatusServerError(true);
        setStatusServerErrorString(statusServer->errorString());
    }
}

void KijangNetworkManager::stop()
{
    if (commServer != nullptr) {
        if (commServer->started()) commServer->quit();
        disconnect(commServer, &KijangTcpServer::forwardHandleRequest, this, &KijangNetworkManager::handleRequest);
        disconnect(this, &KijangNetworkManager::sendResponse, commServer, &KijangTcpServer::forwardSendResponse);
        commServer->deleteLater();
        commServer = nullptr;
    }
    if (statusServer != nullptr) {
        if (statusServer->started()) statusServer->quit();
        statusServer->deleteLater();
        statusServer = nullptr;
    }
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

        resetCommServer();
        if (commServer->start(newCommServerPort)) {
            setCommServerError(false);
        } else {
            setCommServerError(true);
            setCommServerErrorString(commServer->errorString());
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

        resetStatusServer();
        if (statusServer->start(newStatusServerPort)) {
            setStatusServerError(false);
        } else {
            setStatusServerError(true);
            setStatusServerErrorString(statusServer->errorString());
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
    resetStatusServer();
    statusServer->start(0, true);
}

void KijangNetworkManager::commAutodetect()
{
    resetCommServer();
    commServer->start(0, true);
}

void KijangNetworkManager::addModule(KijangModuleHandler *module)
{
    quint16 moduleID = module->module();
    QLoggingCategory network("network");
    if (moduleHandlerList.contains(moduleID)) {
        qWarning(network) << "Could not add module" << moduleID << "as module already exists";
    } else if (moduleID & 32768) {
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
    if (module & 32768) {
        qWarning(network) << "Attempting to remove client module from server";
    } else if (moduleHandlerList.contains(module)) {
        moduleHandlerList.remove(module);
    } else if (module >= 32765 && module <= 32767) {
        qWarning(network) << "Attempting to remove protected modules";
    } else {
        qInfo(network) << "Module" << module << "does not exist";
    }
}

void KijangNetworkManager::resetCommServer() {
    if (commServer != nullptr) {
        if (commServer->started()) commServer->quit();
        disconnect(commServer, &KijangTcpServer::forwardHandleRequest, this, &KijangNetworkManager::handleRequest);
        disconnect(this, &KijangNetworkManager::sendResponse, commServer, &KijangTcpServer::forwardSendResponse);
        commServer->deleteLater();
        commServer = nullptr;
    }
    commServer = new KijangTcpServer(this);
    commServer->setType(KijangTcpServer::ServerType::Communication);
    connect(commServer, &KijangTcpServer::forwardHandleRequest, this, &KijangNetworkManager::handleRequest);
    connect(this, &KijangNetworkManager::sendResponse, commServer, &KijangTcpServer::forwardSendResponse);
}

void KijangNetworkManager::resetStatusServer() {
    if (statusServer != nullptr) {
        if (statusServer->started()) statusServer->quit();
        statusServer->deleteLater();
        statusServer = nullptr;
    }
    statusServer = new KijangTcpServer(this);
    statusServer->setType(KijangTcpServer::ServerType::Status);
}

void KijangNetworkManager::handleRequest(quint32 clientID, Kijang::KijangProtocol request) {
    quint16 moduleID = request.module();
    quint64 requestID = request.requestID();
    if (!moduleHandlerList.contains(moduleID)) {
        Kijang::KijangProtocol response(clientID, 65535, 61442); // Module FFFF, Code F002
        response.setRequestID(requestID);
        emit sendResponse(clientID, response);
        return;
    }

    quint16 code = request.code();
    KijangModuleHandler *handler = moduleHandlerList.value(moduleID);
    if (!handler->canHandleCode(code)) {
        Kijang::KijangProtocol response(clientID, 65535, 61443); // Module FFFF, Code F003
        response.setRequestID(requestID);
        emit sendResponse(clientID, response);
        return;
    }
    handler->handleRequest(request);
}

void KijangNetworkManager::moduleSendResponse(Kijang::KijangProtocol req)
{
    emit sendResponse(req.clientID(), req);
}

void KijangNetworkManager::sendLocalResponse(quint16 src, quint16 target, Kijang::KijangProtocol req)
{
    if (sourceModuleExists(src)) {
        if (!moduleHandlerList.contains(target)) {
            moduleHandlerList.value(src)->modulePresent(target, false);
            return;
        }
        moduleHandlerList.value(target)->handleLocalRequest(src, req);
    }
}

void KijangNetworkManager::checkModulePresent(quint16 src, quint16 module)
{
    if (sourceModuleExists(src)) {
        moduleHandlerList.value(src)->modulePresent(module, moduleHandlerList.contains(module));
    }
}

void KijangNetworkManager::checkCodePresent(quint16 src, quint16 module, quint16 code)
{
    if (sourceModuleExists(src)) {
        moduleHandlerList.value(src)->codePresent(module, code, moduleHandlerList.contains(module) && moduleHandlerList.value(module)->canHandleCode(code));
    }
}

bool KijangNetworkManager::sourceModuleExists(quint16 module)
{
    QLoggingCategory network("network");
    bool response = moduleHandlerList.contains(module);
    if (!response) qWarning(network) << "Module" << module << "claimed to send request but no corresponding module found";
    return response;
}
