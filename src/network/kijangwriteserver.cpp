#include "kijangwriteserver.h"

KijangWriteServer::KijangWriteServer(QObject *parent) : KijangTcpServer(parent)
{
    m_type = "write";
}

void KijangWriteServer::forwardSendResponse(quint32 clientID, KijangProtocol response)
{
    QLoggingCategory network("network");
    if (!clientMap.contains(clientID)) {
        qWarning(network) << "Attempted to send response to non-existent client with ID " << clientID;
        return;
    }
    clientMap.value(clientID)->sendResponse(response);
}

void KijangWriteServer::receivedTermination(quint32 clientID, bool emitEvent)
{
    QLoggingCategory network("network");
    if (!clientMap.contains(clientID)) {
        qDebug(network) << "Client termination request received but no corresponding client ID found, client may have disconnected";
        return;
    }

    WriteClient *client = clientMap.value(clientID);
    disconnect(client, &WriteClient::terminated, this, &KijangWriteServer::receivedTermination);
    client->receiveTermination();
    clientMap.remove(clientID);
    qDebug(network) << "Write client with ID" << clientID << "terminated";
    if (emitEvent) emit clientTerminated(clientID);
}

void KijangWriteServer::clientSelfTerminate(qintptr handle)
{
    QLoggingCategory network("network");
    if (!pendingClients.contains(handle)) {
        qWarning(network) << "Client with handle" << handle << "on write server not found, could not be removed";
        return;
    }

    WriteClient *client = pendingClients.value(handle);
    client->deleteLater();
    pendingClients.remove(handle);
}

void KijangWriteServer::requestVerifyClient(qintptr handle, quint32 clientID)
{
    QLoggingCategory network("network");
    if (clientMap.contains(clientID)) {
        qWarning(network) << "Duplicate write clients detected for client" << clientID << ", removing latest client";
        clientMap.value(clientID)->setClientIsVerified(false);
        return;
    }
    emit verifyClient(handle, clientID);
}

void KijangWriteServer::verifyClientResult(qintptr handle, quint32 clientID, bool isClient)
{
    QLoggingCategory network("network");
    if (isClient) {
        clientMap.insert(clientID, pendingClients.value(handle));
        pendingClients.remove(handle);
        clientMap.value(clientID)->setClientIsVerified(true);
    } else {
        qWarning(network) << "Handle" << handle << "attemped to verify as client" << clientID << "but client already exists, removing";
        pendingClients.value(handle)->setClientIsVerified(false);
    }
}

void KijangWriteServer::incomingConnection(qintptr handle)
{
    QLoggingCategory network("network");
    qDebug(network) << "Incoming connection for write server, handle" << handle;
    WriteClient *client = new WriteClient(nullptr, handle);
    client->setAutoDelete(false); // Manually emit deletion request after run() is completed to allow clientMap to remove client
    connect(client, &WriteClient::selfTerminate, this, &KijangWriteServer::clientSelfTerminate);
    connect(client, &WriteClient::terminated, this, &KijangWriteServer::clientTerminated);
    pendingClients.insert(handle, client);
    pool.start(client);
}
