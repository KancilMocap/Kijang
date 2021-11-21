#ifndef UDPMOTIONLISTENER_H
#define UDPMOTIONLISTENER_H

#include <QObject>
#include <QDebug>
#include "udplistenerinterface.h"

class UdpMotionListener : public QObject, public UdpListenerInterface
{
    Q_OBJECT
    Q_INTERFACES(UdpListenerInterface)
public:
    explicit UdpMotionListener(QObject *parent = nullptr);

signals:

};

#endif // UDPMOTIONLISTENER_H
