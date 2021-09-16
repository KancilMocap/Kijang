#ifndef KIJANGTCPSERVER_H
#define KIJANGTCPSERVER_H

#include "communicationclient.h"
#include "statusclient.h"
#include <QDebug>
#include <QTcpServer>
#include <QThreadPool>
#include <QObject>
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
    bool m_started;
    QThreadPool pool;
    ServerType m_type;
    qint32 m_port;

public slots:
    bool start(quint32 port, bool autoSearch = false);
    void quit();
    int findPort();

    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle) Q_DECL_OVERRIDE;
};

#endif // KIJANGTCPSERVER_H
