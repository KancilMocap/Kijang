#ifndef READCLIENT_H
#define READCLIENT_H

#include <QObject>
#include <QDebug>
#include <QRunnable>
#include <QThread>
#include <QTcpSocket>
#include <QLoggingCategory>
#include <QRandomGenerator>
#include "kijangprotocol.h"

class ReadClient : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit ReadClient(QObject *parent = nullptr, qintptr handle = 0, quint32 id = 0, quint16 writePort = 0);
    ~ReadClient();

    static int INITIAL_TIMEOUT;

signals:
    void terminated(quint32 clientID, bool emitEvent);
    void handleRequest(quint32 clientID, KijangProtocol request);

public slots:
    void receiveTermination();
    void stateChanged(QAbstractSocket::SocketState socketState);

    // QRunnable interface
public:
    void run() Q_DECL_OVERRIDE;
    quint32 clientID() const;

private:
    bool connected;
    bool connected2;
    qintptr handle;
    quint32 m_clientID;
    quint16 m_writePort;
    QTcpSocket *socket;
};

#endif // READCLIENT_H
