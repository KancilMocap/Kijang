#include "kijangreadserver.h"

KijangReadServer::KijangReadServer(QObject *parent) : KijangTcpServer(parent)
{
    m_type = "read";
}

void KijangReadServer::handleRequest(quint32 clientID, KijangProtocol request)
{
    // Forward to network manager
    QLoggingCategory network("network");
    qDebug(network) << "Communication server received request" << request.requestID() << "from client" << clientID << ", forwarding...";
    emit forwardHandleRequest(clientID, request);
}

void KijangReadServer::incomingConnection(qintptr handle)
{
    QLoggingCategory network("network");
    qDebug(network) << "Incoming connection for communication server, handle" << handle;
    quint32 id = random.generate();
    while (clientMap.contains(id)) id = random.generate();
    ReadClient *client = new ReadClient(nullptr, handle, id);
    client->setAutoDelete(false); // Manually emit deletion request after run() is completed to allow clientMap to remove client
    connect(client, &ReadClient::handleRequest, this, &KijangReadServer::handleRequest);
    connect(client, &ReadClient::terminated, this, &KijangReadServer::receivedTermination);
    clientMap.insert(id, client);
    pool.start(client);
    emit readClientAdded(id, client);
}

void KijangReadServer::setWritePort(quint16 newWritePort)
{
    m_writePort = newWritePort;
}

void KijangReadServer::receivedTermination(quint32 clientID, bool emitEvent)
{
    QLoggingCategory network("network");
    if (!clientMap.contains(clientID)) {
        qDebug(network) << "Client termination request received but no corresponding client ID found, client may have disconnected";
        return;
    }

    ReadClient *client = clientMap.value(clientID);
    disconnect(client, &ReadClient::handleRequest, this, &KijangReadServer::handleRequest);
    disconnect(client, &ReadClient::terminated, this, &KijangReadServer::receivedTermination);
    client->receiveTermination();
    clientMap.remove(clientID);
    qDebug(network) << "Read client with ID" << clientID << "terminated";
    if (emitEvent) emit clientTerminated(clientID);
}
