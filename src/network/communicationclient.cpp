#include "communicationclient.h"

int CommunicationClient::INITIAL_TIMEOUT = 3000;

CommunicationClient::CommunicationClient(QObject *parent, qintptr handle) : QObject(parent)
{
    QLoggingCategory network("network");
    qDebug(network) << "Communication client with handle" << handle << "constructed";
    this->handle = handle;
    connected = false;
}

CommunicationClient::~CommunicationClient()
{
    QLoggingCategory network("network");
    qDebug(network) << "Communication client with handle" << handle << "deconstructed";
}

void CommunicationClient::stateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::SocketState::ClosingState) {
        connected = false;
        emit terminated(nullptr);
    }
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
    connected = true;
    while (connected) {
        socket.waitForReadyRead();
        QByteArray request = socket.readAll();
        Kijang::KijangProtocol packet(request);
        if (packet.exceptionInfo()) {
            qWarning(network) << "Invalid KijangProtocol packet received from socket " << handle;
            continue;
        }

        // TODO: Handle packet
    }
}

quint32 CommunicationClient::clientID() const
{
    return m_clientID;
}

void CommunicationClient::setClientID(quint32 newClientID)
{
    m_clientID = newClientID;
}
