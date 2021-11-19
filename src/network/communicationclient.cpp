#include "communicationclient.h"

int CommunicationClient::INITIAL_TIMEOUT = 3000;

CommunicationClient::CommunicationClient(QObject *parent, qintptr handle, quint32 id) : QObject(parent)
{
    QLoggingCategory network("network");
    qDebug(network) << "Communication client with handle" << handle << "constructed, assigned to ID" << id;
    this->handle = handle;
    this->m_clientID = id;
    connected = false;
}

CommunicationClient::~CommunicationClient()
{
    QLoggingCategory network("network");
    qDebug(network) << "Communication client with ID" << m_clientID << "deconstructed";
}

void CommunicationClient::stateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::SocketState::ClosingState) {
        connected = false;
    }
}

void CommunicationClient::sendResponse(Kijang::KijangProtocol response)
{
    QLoggingCategory network("network");
    if (socket.state() != QTcpSocket::ConnectedState) {
        qWarning(network) << "Socket" << m_clientID << "not connected, response could not be sent";
        return;
    }
    qDebug(network) << "Response sent to" << m_clientID << ":" << response.toByteArray().toHex();
    socket.write(response.toByteArray());
    socket.waitForBytesWritten();
}

void CommunicationClient::run()
{
    QLoggingCategory network("network");
    if (!socket.setSocketDescriptor(handle)) {
        qCritical(network) << "Unable to create communication socket due to" << socket.errorString();
        return;
    }
    socket.waitForConnected();
    connect(&socket, &QTcpSocket::stateChanged, this, &CommunicationClient::stateChanged);
    QRandomGenerator random;
    m_clientID = random.generate();
    connected = true;
    while (connected) {
        socket.waitForReadyRead();
        QByteArray request = socket.readAll();
        if (request.length() == 0) continue;
        qDebug(network) << "Current request from" << m_clientID << ":" << request.toHex();
        Kijang::KijangProtocol packet(request, m_clientID);
        if (packet.exceptionInfo()) {
            qWarning(network) << "Invalid KijangProtocol packet received from socket " << handle << " with exception info " << packet.errorString();
            continue;
        }
        emit handleRequest(m_clientID, packet);
    }
    emit terminated(m_clientID);
}

quint32 CommunicationClient::clientID() const
{
    return m_clientID;
}
