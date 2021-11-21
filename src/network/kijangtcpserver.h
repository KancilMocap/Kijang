#ifndef KIJANGTCPSERVER_H
#define KIJANGTCPSERVER_H

#include "readclient.h"
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
    explicit KijangTcpServer(QObject *parent=nullptr);
    static int maxThreadCount;
    qint16 port() const;
    bool started() const;

protected:
    bool m_started = false;
    QThreadPool pool;
    QRandomGenerator random;
    qint16 m_port;

public slots:
    bool start(quint32 port, bool autoSearch = false);
    void quit();
    int findPort();

    // QTcpServer interface
protected:
    virtual void incomingConnection(qintptr handle) = 0;
    QString m_type;
};

#endif // KIJANGTCPSERVER_H
