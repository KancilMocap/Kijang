#ifndef KIJANGSTATUSSERVER_H
#define KIJANGSTATUSSERVER_H

#include "kijangtcpserver.h"
#include "statusclient.h"
#include <QObject>

class KijangStatusServer : public KijangTcpServer
{
    Q_OBJECT
public:
    explicit KijangStatusServer(QObject *parent = nullptr);

    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle) Q_DECL_OVERRIDE;

private:
    QList<StatusClient *> clientList;
};

#endif // KIJANGSTATUSSERVER_H
