#ifndef STATUSCLIENT_H
#define STATUSCLIENT_H

#include <QObject>
#include <QDebug>
#include <QRunnable>
#include <QThread>
#include <QTcpSocket>

class StatusClient : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit StatusClient(QObject *parent = nullptr, qintptr handle = 0);

signals:


    // QRunnable interface
public:
    void run() Q_DECL_OVERRIDE;

private:
    qintptr handle;
};

#endif // STATUSCLIENT_H
