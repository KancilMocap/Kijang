#include "kijangtcpserver.h"

int KijangTcpServer::maxThreadCount = 50;

KijangTcpServer::KijangTcpServer(ServerType type, QObject *parent) : QTcpServer(parent)
{
    pool.setMaxThreadCount(KijangTcpServer::maxThreadCount);
    m_type = type;
}

bool KijangTcpServer::start(quint16 port, bool autoSearch)
{
    return false;
}

void KijangTcpServer::quit()
{

}

int KijangTcpServer::findPort()
{
    return 0;
}


void KijangTcpServer::incomingConnection(qintptr handle)
{

}
