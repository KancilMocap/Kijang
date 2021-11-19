#include "kijangtcpserver.h"

int KijangTcpServer::maxThreadCount = 50;

KijangTcpServer::KijangTcpServer(QObject *parent) : QTcpServer(parent)
{
    pool.setMaxThreadCount(KijangTcpServer::maxThreadCount);
}

QString KijangTcpServer::typeString()
{
    return m_type == KijangTcpServer::ServerType::Communication ? "communication" : "status";
}

void KijangTcpServer::setType(ServerType newType)
{
    m_type = newType;
}

qint16 KijangTcpServer::port() const
{
    return m_port;
}

bool KijangTcpServer::started() const
{
    return m_started;
}

bool KijangTcpServer::start(quint32 port, bool autoSearch)
{
    // Pool should be cleared when quit() was called
    QLoggingCategory network("network");
    if (m_started) quit();
    m_port = port;
    if (autoSearch) m_port = findPort();
    if (m_port) {
        if (autoSearch) {
            m_started = true;
            return true;
        } else if (this->listen(QHostAddress::Any, port)) {
            qInfo(network) << typeString() << "server started on port" << port;
            m_started = true;
            return true;
        } else {
            qCritical(network) << "Unable to start" << typeString() << "server due to" << this->errorString();
            m_started = false;
            return false;
        }
    }
    else {
        qCritical(network) << "Unable to start" << typeString() << "server due to invalid port";
        m_started = false;
        return false;
    }
}

void KijangTcpServer::handleRequest(quint32 clientID, Kijang::KijangProtocol request)
{
    // Forward to network manager
    emit forwardHandleRequest(clientID, request);
}

void KijangTcpServer::forwardSendResponse(quint32 clientID, Kijang::KijangProtocol response)
{
    QLoggingCategory network("network");
    if (!clientMap.contains(clientID)) {
        qWarning(network) << "Attempted to send response to non-existent client with ID " << clientID;
        return;
    }
    clientMap.value(clientID)->sendResponse(response);
}

void KijangTcpServer::clientTerminated(quint32 clientID)
{
    QLoggingCategory network("network");
    if (!clientMap.contains(clientID)) {
        qWarning(network) << "Client termination request received but no corresponding client ID found";
        return;
    }

    clientMap.value(clientID)->deleteLater();
    clientMap.remove(clientID);
    qDebug(network) << "Communication client with ID" << clientID << "terminated";
}

void KijangTcpServer::quit()
{
    QLoggingCategory network("network");
    pool.clear();
    if (m_started) {
        m_started = false;
        this->close();
        qInfo(network) << typeString() << "server stopped";
    } else {
        qInfo(network) << typeString() << "server stop called but server already stopped";
    }
}

int KijangTcpServer::findPort()
{
    // Also binds to the port
    QLoggingCategory network("network");
    // 1-1024 not included in auto search
    for (quint32 i = 1025; i <= 65535; i++) {
        if (this->listen(QHostAddress::Any, i)) {
            qInfo(network) << "Auto search found port" << i << "," << typeString() << "server started";
            return i;
        } else {
            qDebug(network) << "Auto search -" << typeString() << ": Attempted binding on port" << i << "failed due to" << errorString();
        }
    }
    qWarning(network) << "Unable to bind to any ports on TCP network for" << typeString() << "server";
    return 0;
}

void KijangTcpServer::incomingConnection(qintptr handle)
{
    QLoggingCategory network("network");
    qDebug(network) << "Incoming connection for" << typeString() << "server, handle" << handle;
    if (m_type == KijangTcpServer::ServerType::Communication) {
        quint32 id = random.generate();
        CommunicationClient *client = new CommunicationClient(nullptr, handle, id);
        client->setAutoDelete(false); // Manually emit deletion request after run() is completed to allow clientMap to remove client
        connect(client, &CommunicationClient::terminated, this, &KijangTcpServer::clientTerminated);
        clientMap.insert(id, client);
        pool.start(client);
    } else {
        QRunnable* client;
        client = new StatusClient(nullptr, handle);
        client->setAutoDelete(true);
        pool.start(client);
    }
}
