#ifndef KIJANGREADSERVER_H
#define KIJANGREADSERVER_H

#include "kijangtcpserver.h"
#include <QObject>

class KijangReadServer : public KijangTcpServer
{
    Q_OBJECT
public:
    explicit KijangReadServer(QObject *parent = nullptr);

    void setWritePort(quint16 newWritePort);

signals:
    void forwardHandleRequest(quint32 clientID, KijangProtocol request);
    void clientTerminated(quint32 clientID);
    void readClientAdded(quint32 clientID, ReadClient *client);

public slots:
    void handleRequest(quint32 clientID, KijangProtocol request);
    void receivedTermination(quint32 clientID, bool emitEvent);

    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle) Q_DECL_OVERRIDE;

private:
    quint16 m_writePort;
    QMap<quint32, ReadClient *> clientMap;
};

#endif // KIJANGREADSERVER_H
