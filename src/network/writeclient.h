#ifndef WRITECLIENT_H
#define WRITECLIENT_H

#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include <QSemaphore>
#include <QLoggingCategory>
#include <QRandomGenerator>
#include "kijangprotocol.h"

class WriteClient : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit WriteClient(QObject *parent = nullptr, qintptr handle = 0);
    ~WriteClient();

signals:
    void selfTerminate(qintptr handle);
    void terminated(quint32 clientID, bool emitEvent);
    void requestVerifyClient(qintptr handle, quint32 clientID);

public slots:
    void receiveTermination();
    void stateChanged(QAbstractSocket::SocketState socketState);
    void sendResponse(KijangProtocol response);

private:
    bool connected;
    bool connected2;
    bool m_clientIsVerified;
    qintptr handle;
    quint32 m_clientID;
    QTcpSocket *socket;
    // QEventLoop could not be used as QTcpSocket cannot handle multithreading
    QSemaphore verifiedClientSemaphore; // Used as mutex; locked by default until write server confirms that client actually exists
    QSemaphore pendingResponsesSemaphore; // Adds one when a pending response is added, removes one when a pending response is sent
    QMutex pendingResponsesMutex; // Locks pendingResponses from simultaneous editing
    QList<QByteArray> pendingResponses;

    // QRunnable interface
public:
    void run() Q_DECL_OVERRIDE;
    quint32 clientID() const;
    void setClientIsVerified(bool newClientIsVerified);
};

#endif // WRITECLIENT_H
