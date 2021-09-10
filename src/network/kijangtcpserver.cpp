#include "kijangtcpserver.h"

int KijangTcpServer::maxThreadCount = 50;

KijangTcpServer::KijangTcpServer(QObject *parent) : QTcpServer(parent)
{
    pool.setMaxThreadCount(KijangTcpServer::maxThreadCount);
}

void KijangTcpServer::setType(ServerType newType)
{
    m_type = newType;
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
