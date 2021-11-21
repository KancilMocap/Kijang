#include "kijangtcpserver.h"

int KijangTcpServer::maxThreadCount = 50;

KijangTcpServer::KijangTcpServer(QObject *parent) : QTcpServer(parent)
{
    pool.setMaxThreadCount(KijangTcpServer::maxThreadCount);
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
            qInfo(network) << m_type << "server started on port" << port;
            m_started = true;
            return true;
        } else {
            qCritical(network) << "Unable to start" << m_type << "server due to" << this->errorString();
            m_started = false;
            return false;
        }
    }
    else {
        qCritical(network) << "Unable to start" << m_type << "server due to invalid port";
        m_started = false;
        return false;
    }
}

void KijangTcpServer::quit()
{
    QLoggingCategory network("network");
    pool.clear();
    if (m_started) {
        m_started = false;
        this->close();
        qInfo(network) << m_type << "server stopped";
    } else {
        qInfo(network) << m_type << "server stop called but server already stopped";
    }
}

int KijangTcpServer::findPort()
{
    // Also binds to the port
    QLoggingCategory network("network");
    // 1-1024 not included in auto search
    for (quint32 i = 1025; i <= 65535; i++) {
        if (this->listen(QHostAddress::Any, i)) {
            qInfo(network) << "Auto search found port" << i << "," << m_type << "server started";
            return i;
        } else {
            qDebug(network) << "Auto search -" << m_type << ": Attempted binding on port" << i << "failed due to" << errorString();
        }
    }
    qWarning(network) << "Unable to bind to any ports on TCP network for" << m_type << "server";
    return 0;
}
