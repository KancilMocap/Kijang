#ifndef KIJANGWRITESERVER_H
#define KIJANGWRITESERVER_H

#include "kijangtcpserver.h"
#include "writeclient.h"
#include <QObject>

class KijangWriteServer : public KijangTcpServer
{
    Q_OBJECT
public:
    explicit KijangWriteServer(QObject *parent = nullptr);

signals:
    void clientTerminated(quint32 clientID);
    void verifyClient(qintptr handle, quint32 clientID);

public slots:
    void forwardSendResponse(quint32 clientID, KijangProtocol response);
    void receivedTermination(quint32 clientID, bool emitEvent);
    void clientSelfTerminate(qintptr handle);
    void requestVerifyClient(qintptr handle, quint32 clientID);
    void verifyClientResult(qintptr handle, quint32 clientID, bool isClient);

    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle) Q_DECL_OVERRIDE;

private:
    QMap<quint32, WriteClient *> clientMap;
    QMap<qintptr, WriteClient *> pendingClients;
};

#endif // KIJANGWRITESERVER_H
