#include "readclient.h"

int ReadClient::INITIAL_TIMEOUT = 3000;

ReadClient::ReadClient(QObject *parent, qintptr handle, quint32 id, quint16 writePort) : QObject(parent)
{
    QLoggingCategory network("network");
    qDebug(network) << "Read client with handle" << handle << "constructed, assigned to ID" << id;
    this->handle = handle;
    this->m_clientID = id;
    this->m_writePort = writePort;
    connected = false;
    connected2 = false;
}

ReadClient::~ReadClient()
{
    QLoggingCategory network("network");
    qDebug(network) << "Communication client with ID" << m_clientID << "deconstructed";
}

void ReadClient::stateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::SocketState::ClosingState) {
        connected = false;
        connected2 = false;
    }
}

void ReadClient::run()
{
    socket = new QTcpSocket(nullptr);
    QLoggingCategory network("network");
    if (!socket->setSocketDescriptor(handle)) {
        qCritical(network) << "Unable to create communication socket due to" << socket->errorString();
        return;
    }
    socket->waitForConnected();
    connect(socket, &QTcpSocket::stateChanged, this, &ReadClient::stateChanged);
    connected = true;
    connected2 = true;

    // Write port
    QByteArray response;
    QDataStream stream(&response, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << m_writePort;
    socket->write(response);
    socket->waitForBytesWritten();

    while (connected && connected2) {
        socket->waitForReadyRead();
        QByteArray request = socket->readAll();
        if (request.length() == 0) continue;
        qDebug(network) << "Current request from" << m_clientID << ":" << request.toHex();
        KijangProtocol packet(request, m_clientID);
        if (packet.exceptionInfo()) {
            qWarning(network) << "Invalid KijangProtocol packet received from socket " << handle << " with exception info " << packet.errorString();
            continue;
        }
        emit handleRequest(m_clientID, packet);
    }
    if (connected) socket->close();
    else emit terminated(m_clientID, true);
    deleteLater();
}

quint32 ReadClient::clientID() const
{
    return m_clientID;
}

void ReadClient::receiveTermination() {
    connected2 = false;
}
