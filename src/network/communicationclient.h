#ifndef COMMUNICATIONCLIENT_H
#define COMMUNICATIONCLIENT_H

#include <QObject>
#include <QDebug>
#include <QRunnable>
#include <QThread>
#include <QTcpSocket>
#include <QLoggingCategory>

class CommunicationClient : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit CommunicationClient(QObject *parent = nullptr, qintptr handle = 0);
    ~CommunicationClient();

    static int INITIAL_TIMEOUT;

signals:


    // QRunnable interface
public:
    void run() Q_DECL_OVERRIDE;

private:
    qintptr handle;
    QTcpSocket socket;
};

#endif // COMMUNICATIONCLIENT_H
