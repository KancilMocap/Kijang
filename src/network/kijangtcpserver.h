#ifndef KIJANGTCPSERVER_H
#define KIJANGTCPSERVER_H

#include "communicationclient.h"
#include "statusclient.h"
#include <QDebug>
#include <QTcpServer>
#include <QThreadPool>
#include <QObject>
#include <QRandomGenerator>
#include <QLoggingCategory>

class KijangTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    enum ServerType {
        Communication,
        Status
    };
    explicit KijangTcpServer(QObject *parent=nullptr);
    static int maxThreadCount;

    QString typeString();
    void setType(ServerType newType);

    qint16 port() const;

    bool started() const;

private:
    bool m_started = false;
    QMap<quint32, CommunicationClient *> clientMap;
    QThreadPool pool;
    QRandomGenerator random;
    ServerType m_type;
    qint16 m_port;

signals:
    void forwardHandleRequest(quint32 clientID, Kijang::KijangProtocol request);

public slots:
    bool start(quint32 port, bool autoSearch = false);
    void handleRequest(quint32 clientID, Kijang::KijangProtocol request);
    void forwardSendResponse(quint32 clientID, Kijang::KijangProtocol response);
    void clientTerminated(quint32 clientID);
    void quit();
    int findPort();

    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle) Q_DECL_OVERRIDE;
};

#endif // KIJANGTCPSERVER_H
