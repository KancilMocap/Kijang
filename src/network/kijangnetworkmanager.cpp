#include "kijangnetworkmanager.h"

QString KijangNetworkManager::settingsFile = QDir::currentPath() + QDir::separator() + "settings.ini";

KijangNetworkManager::KijangNetworkManager(QObject *parent) : QObject(parent)
{
    QSettings settings(settingsFile, QSettings::IniFormat);
    settings.beginGroup("network");
    QLoggingCategory network("network");
    qDebug(network) << "Network manager constructed";
    bool ok;
    m_readServerPort = settings.value("read_port", 4318).toUInt(&ok);
    if (!ok) m_readServerPort = 4318;
    m_writeServerPort = settings.value("write_port", 4319).toUInt(&ok);
    if (!ok) m_writeServerPort = 4319;
    m_statusServerPort = settings.value("status_port", 4320).toUInt(&ok);
    if (!ok) m_statusServerPort = 4320;
    settings.endGroup();

    addModuleAfterCheck(dynamic_cast<KijangModuleHandler *>(&module7FFF));
    addModuleAfterCheck(dynamic_cast<KijangModuleHandler *>(&module7FFE));
}

KijangNetworkManager::~KijangNetworkManager()
{
    QSettings settings(settingsFile, QSettings::IniFormat);
    settings.beginGroup("network");
    settings.setValue("read_port", m_readServerPort);
    settings.setValue("write_port", m_writeServerPort);
    settings.setValue("status_port", m_statusServerPort);
    settings.endGroup();
    settings.sync();
}

void KijangNetworkManager::start()
{
    resetWriteServer();
    resetReadServer();
    resetStatusServer();
    if (readServer->start(m_readServerPort)) {
        setReadServerError(false);
    } else {
        setReadServerError(true);
        setReadServerErrorString(readServer->errorString());
    }
    if (writeServer->start(m_writeServerPort)) {
        setWriteServerError(false);
    } else {
        setWriteServerError(true);
        setWriteServerErrorString(writeServer->errorString());
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
    if (readServer != nullptr) {
        if (readServer->started()) readServer->quit();
        disconnect(readServer, &KijangReadServer::forwardHandleRequest, this, &KijangNetworkManager::handleRequest);
        readServer->deleteLater();
        readServer = nullptr;
    }
    if (writeServer != nullptr) {
        if (writeServer->started()) writeServer->quit();
        disconnect(this, &KijangNetworkManager::sendResponse, writeServer, &KijangWriteServer::forwardSendResponse);
        writeServer->deleteLater();
        writeServer = nullptr;
    }
    if (statusServer != nullptr) {
        if (statusServer->started()) statusServer->quit();
        statusServer->deleteLater();
        statusServer = nullptr;
    }
}

quint16 KijangNetworkManager::readServerPort() const
{
    return m_readServerPort;
}

void KijangNetworkManager::setReadServerPort(quint16 newReadServerPort)
{
    if (newReadServerPort != m_readServerPort) {
        m_readServerPort = newReadServerPort;
        emit readServerPortChanged();

        resetWriteServer();
        resetReadServer();
        if (readServer->start(newReadServerPort)) {
            setReadServerError(false);
        } else {
            setReadServerError(true);
            setReadServerErrorString(readServer->errorString());
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

bool KijangNetworkManager::readServerError() const
{
    return m_readServerError;
}

void KijangNetworkManager::setReadServerError(bool newReadServerError)
{
    if (m_readServerError == newReadServerError)
        return;
    m_readServerError = newReadServerError;
    emit readServerErrorChanged();
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

const QString &KijangNetworkManager::readServerErrorString() const
{
    return m_readServerErrorString;
}

void KijangNetworkManager::setReadServerErrorString(const QString &newReadServerErrorString)
{
    if (m_readServerErrorString == newReadServerErrorString)
        return;
    m_readServerErrorString = newReadServerErrorString;
    emit readServerErrorStringChanged();
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

void KijangNetworkManager::readAutoDetect()
{
    resetReadServer();
    readServer->start(0, true);
    resetWriteServer();
}

void KijangNetworkManager::writeAutoDetect()
{
    resetWriteServer();
    writeServer->start(0, true);
    resetReadServer();
}

void KijangNetworkManager::addModule(KijangModuleHandler *module)
{
    quint16 moduleID = module->module();
    QLoggingCategory network("network");
    if (moduleHandlerList.contains(moduleID)) {
        qWarning(network) << "Could not add module" << moduleID << "as module already exists";
    } else if (moduleID & 32768) {
        qWarning(network) << "Attempting to add client module to server for module" << moduleID;
    } else if (moduleID == 32766 || moduleID == 32767 ) {
        qWarning(network) << "Attempting to add protected modules";
    } else {
        addModuleAfterCheck(module);
    }
}

void KijangNetworkManager::removeModule(quint16 module)
{
    QLoggingCategory network("network");
    if (module & 32768) {
        qWarning(network) << "Attempting to remove client module from server";
    } else if (moduleHandlerList.contains(module)) {
        KijangModuleHandler *currentModule = moduleHandlerList.value(module);
        disconnect(dynamic_cast<QObject*>(currentModule), SIGNAL(sendResponse(KijangProtocol)), this, SLOT(moduleSendResponse(KijangProtocol)));
        disconnect(dynamic_cast<QObject*>(currentModule), SIGNAL(sendLocalResponse(quint16,quint16,KijangProtocol)), this, SLOT(sendLocalResponse(quint16,quint16,KijangProtocol)));
        disconnect(dynamic_cast<QObject*>(currentModule), SIGNAL(checkModulePresent(quint16,quint16)), this, SLOT(checkModulePresent(quint16,quint16)));
        disconnect(dynamic_cast<QObject*>(currentModule), SIGNAL(checkCodePresent(quint16,quint16,quint16)), this, SLOT(checkCodePresent(quint16,quint16,quint16)));
        moduleHandlerList.remove(module);
    } else if (module == 32766 || module == 32767) {
        qWarning(network) << "Attempting to remove protected modules";
    } else {
        qInfo(network) << "Module" << module << "does not exist";
    }
}

void KijangNetworkManager::resetReadServer() {
    if (readServer != nullptr) {
        if (readServer->started()) readServer->quit();
        disconnect(readServer, &KijangReadServer::forwardHandleRequest, this, &KijangNetworkManager::handleRequest);
        disconnect(readServer, &KijangReadServer::readClientAdded, this, &KijangNetworkManager::readClientAdded);
        readServer->deleteLater();
        readServer = nullptr;
    }
    readServer = new KijangReadServer(this);
    readServer->setWritePort(m_writeServerPort);
    connect(readServer, &KijangReadServer::forwardHandleRequest, this, &KijangNetworkManager::handleRequest);
    connect(readServer, &KijangReadServer::readClientAdded, this, &KijangNetworkManager::readClientAdded);
}

void KijangNetworkManager::resetWriteServer()
{
    if (writeServer != nullptr) {
        if (writeServer->started()) writeServer->quit();
        disconnect(this, &KijangNetworkManager::sendResponse, writeServer, &KijangWriteServer::forwardSendResponse);
        disconnect(writeServer, &KijangWriteServer::verifyClient, this, &KijangNetworkManager::writeVerifyClient);
        writeServer->deleteLater();
        writeServer = nullptr;
    }
    writeServer = new KijangWriteServer(this);
    connect(this, &KijangNetworkManager::sendResponse, writeServer, &KijangWriteServer::forwardSendResponse);
    connect(writeServer, &KijangWriteServer::verifyClient, this, &KijangNetworkManager::writeVerifyClient);
}

void KijangNetworkManager::resetStatusServer() {
    if (statusServer != nullptr) {
        if (statusServer->started()) statusServer->quit();
        statusServer->deleteLater();
        statusServer = nullptr;
    }
    statusServer = new KijangStatusServer(this);
}

const QString &KijangNetworkManager::writeServerErrorString() const
{
    return m_writeServerErrorString;
}

void KijangNetworkManager::setWriteServerErrorString(const QString &newWriteServerErrorString)
{
    if (m_writeServerErrorString == newWriteServerErrorString)
        return;
    m_writeServerErrorString = newWriteServerErrorString;
    emit writeServerErrorStringChanged();
}

bool KijangNetworkManager::writeServerError() const
{
    return m_writeServerError;
}

void KijangNetworkManager::setWriteServerError(bool newWriteServerError)
{
    if (m_writeServerError == newWriteServerError)
        return;
    m_writeServerError = newWriteServerError;
    emit writeServerErrorChanged();
}

quint16 KijangNetworkManager::writeServerPort() const
{
    return m_writeServerPort;
}

void KijangNetworkManager::setWriteServerPort(quint16 newWriteServerPort)
{
    if (newWriteServerPort != m_writeServerPort) {
        m_writeServerPort = newWriteServerPort;
        emit writeServerPortChanged();

        resetWriteServer();
        resetReadServer();
        if (writeServer->start(newWriteServerPort)) {
            setWriteServerError(false);
        } else {
            setWriteServerError(true);
            setWriteServerErrorString(writeServer->errorString());
        }
    }
}

void KijangNetworkManager::handleRequest(quint32 clientID, KijangProtocol request) {
    QLoggingCategory network("network");
    quint16 moduleID = request.module();
    quint64 requestID = request.requestID();
    qDebug(network) << "Handling request" << requestID << "from client" << clientID;
    if (!moduleHandlerList.contains(moduleID)) {
        KijangProtocol response(clientID, 65535, 61442); // Module FFFF, Code F002
        response.setRequestID(requestID);
        emit sendResponse(clientID, response);
        return;
    }

    quint16 code = request.code();
    KijangModuleHandler *handler = moduleHandlerList.value(moduleID);
    if (!handler->canHandleCode(code)) {
        KijangProtocol response(clientID, 65535, 61443); // Module FFFF, Code F003
        response.setRequestID(requestID);
        emit sendResponse(clientID, response);
        return;
    }
    handler->handleRequest(request);
}

void KijangNetworkManager::readClientTerminated(quint32 clientID)
{
    writeServer->receivedTermination(clientID, false);
    if (clientList.contains(clientID)) clientList.remove(clientID);
}

void KijangNetworkManager::readClientAdded(quint32 clientID)
{
    clientList.append(clientID);
}

void KijangNetworkManager::writeClientTerminated(quint32 clientID)
{
    readServer->receivedTermination(clientID, false);
    if (clientList.contains(clientID)) clientList.remove(clientID);
}

void KijangNetworkManager::writeVerifyClient(qintptr handle, quint32 clientID)
{
    writeServer->verifyClientResult(handle, clientID, clientList.contains(clientID));
}

void KijangNetworkManager::moduleSendResponse(KijangProtocol req)
{
    emit sendResponse(req.clientID(), req);
}

void KijangNetworkManager::sendLocalResponse(quint16 src, quint16 target, KijangProtocol req)
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

void KijangNetworkManager::addModuleAfterCheck(KijangModuleHandler *module)
{
    connect(dynamic_cast<QObject*>(module), SIGNAL(sendResponse(KijangProtocol)), this, SLOT(moduleSendResponse(KijangProtocol)));
    connect(dynamic_cast<QObject*>(module), SIGNAL(sendLocalResponse(quint16,quint16,KijangProtocol)), this, SLOT(sendLocalResponse(quint16,quint16,KijangProtocol)));
    connect(dynamic_cast<QObject*>(module), SIGNAL(checkModulePresent(quint16,quint16)), this, SLOT(checkModulePresent(quint16,quint16)));
    connect(dynamic_cast<QObject*>(module), SIGNAL(checkCodePresent(quint16,quint16,quint16)), this, SLOT(checkCodePresent(quint16,quint16,quint16)));
    moduleHandlerList.insert(module->module(), module);
}
