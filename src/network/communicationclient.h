#ifndef COMMUNICATIONCLIENT_H
#define COMMUNICATIONCLIENT_H

#include <QObject>
#include <QDebug>
#include <QRunnable>
#include <QThread>
#include <QTcpSocket>
#include <QEventLoop>
#include <QLoggingCategory>
#include <QRandomGenerator>
#include "../../inc/kijangProtocol/kijangprotocol.h"

class CommunicationClient : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit CommunicationClient(QObject *parent = nullptr, qintptr handle = 0, quint32 id = 0);
    ~CommunicationClient();

    static int INITIAL_TIMEOUT;

signals:
    void terminated(quint32 clientID);
    void handleRequest(quint32 clientID, Kijang::KijangProtocol request);

public slots:
    void stateChanged(QAbstractSocket::SocketState socketState);
    void sendResponse(Kijang::KijangProtocol response);

    // QRunnable interface
public:
    void run() Q_DECL_OVERRIDE;
    quint32 clientID() const;

private:
    bool connected;
    qintptr handle;
    quint32 m_clientID;
    QTcpSocket socket;
};

#endif // COMMUNICATIONCLIENT_H
