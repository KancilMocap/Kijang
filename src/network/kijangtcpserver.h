#ifndef KIJANGTCPSERVER_H
#define KIJANGTCPSERVER_H

#include <QDebug>
#include <QTcpServer>
#include <QThreadPool>
#include <QObject>

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

    void setType(ServerType newType);

private:
    QThreadPool pool;
    ServerType m_type;

public slots:
    bool start(quint16 port, bool autoSearch = false);
    void quit();
    int findPort();

    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle) Q_DECL_OVERRIDE;
};

#endif // KIJANGTCPSERVER_H
