#ifndef UDPVIDEOLISTENER_H
#define UDPVIDEOLISTENER_H

#include <QObject>
#include <QDebug>
#include "udplistenerinterface.h"

class UdpVideoListener : public QObject, public UdpListenerInterface
{
    Q_OBJECT
public:
    explicit UdpVideoListener(QObject *parent = nullptr);

signals:

};

#endif // UDPVIDEOLISTENER_H
