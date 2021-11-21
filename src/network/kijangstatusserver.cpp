#include "kijangstatusserver.h"

KijangStatusServer::KijangStatusServer(QObject *parent) : KijangTcpServer(parent)
{
    m_type = "status";
}


void KijangStatusServer::incomingConnection(qintptr handle)
{
    QLoggingCategory network("network");
    qDebug(network) << "Incoming connection for status server, handle" << handle;
    StatusClient* client;
    client = new StatusClient(nullptr, handle);
    client->setAutoDelete(false);
    clientList.append(client);
    pool.start(client);
}
