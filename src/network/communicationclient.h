#ifndef COMMUNICATIONCLIENT_H
#define COMMUNICATIONCLIENT_H

#include <QObject>
#include <QDebug>
#include <QRunnable>
#include <QThread>
#include <QTcpSocket>

class CommunicationClient : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit CommunicationClient(QObject *parent = nullptr, qintptr handle = 0);

signals:


    // QRunnable interface
public:
    void run() Q_DECL_OVERRIDE;

private:
    qintptr handle;
};

#endif // COMMUNICATIONCLIENT_H
