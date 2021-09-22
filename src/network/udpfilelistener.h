#ifndef UDPFILELISTENER_H
#define UDPFILELISTENER_H

#include <QObject>

class UdpFileLIstener : public QObject
{
    Q_OBJECT
public:
    explicit UdpFileLIstener(QObject *parent = nullptr);

signals:

};

#endif // UDPFILELISTENER_H
