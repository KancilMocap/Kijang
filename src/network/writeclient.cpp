#include "writeclient.h"

WriteClient::WriteClient(QObject *parent, qintptr handle) : QObject(parent)
{
    QLoggingCategory network("network");
    qDebug(network) << "Read client with handle" << handle << "constructed, awaiting ID";
    this->handle = handle;
    connected = false;
    connected2 = false;
    m_clientIsVerified = false;
}

WriteClient::~WriteClient()
{

}

void WriteClient::receiveTermination()
{
    connected2 = false;
}

void WriteClient::stateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::SocketState::ClosingState) {
        connected = false;
        connected2 = false;
    }
}

void WriteClient::sendResponse(KijangProtocol response)
{
    QLoggingCategory network("network");
    if (!socket || socket->state() != QTcpSocket::ConnectedState) {
        qWarning(network) << "Socket" << m_clientID << "not connected, response could not be sent";
        return;
    }
    qDebug(network) << "Response sent to" << m_clientID << ":" << response.toByteArray().toHex();
    pendingResponsesMutex.lock();
    pendingResponses.append(response.toByteArray());
    pendingResponsesMutex.unlock();
    pendingResponsesSemaphore.release();
}

void WriteClient::setClientIsVerified(bool newClientIsVerified)
{
    m_clientIsVerified = newClientIsVerified;
    verifiedClientSemaphore.release();
}

quint32 WriteClient::clientID() const
{
    return m_clientID;
}

void WriteClient::run()
{
    socket = new QTcpSocket(nullptr);
    QLoggingCategory network("network");
    if (!socket->setSocketDescriptor(handle)) {
        qCritical(network) << "Unable to create communication socket due to" << socket->errorString();
        return;
    }
    socket->waitForConnected();
    connect(socket, &QTcpSocket::stateChanged, this, &WriteClient::stateChanged);
    connected = true;
    connected2 = true;

    // Wait for identification
    socket->waitForReadyRead(50000); // 5 seconds
    QByteArray id = socket->readAll();
    if (id.length() != 4) {
        qCritical(network) << "Client attempted to send invalid data or no data received, disconnecting from write server";
        socket->disconnect();
        emit selfTerminate(handle);
        return;
    }
    m_clientID = qFromBigEndian<quint32>(id);
    emit requestVerifyClient(handle, m_clientID);
    verifiedClientSemaphore.acquire();
    if (!m_clientIsVerified) {
        qCritical(network) << "No client with corresponding ID exists, disconnecting";
        socket->disconnect();
        emit selfTerminate(handle);
        return;
    }

    while (connected && connected2) {
        // Wait for write
        pendingResponsesSemaphore.acquire(1);
        if (!connected) break;
        pendingResponsesMutex.lock();
        QByteArray response = pendingResponses.at(0);
        pendingResponses.pop_front();
        pendingResponsesMutex.unlock();
        socket->write(response);
        socket->waitForBytesWritten();
    }

    if (connected) socket->close();
    else emit terminated(m_clientID, true);
    deleteLater();
}
