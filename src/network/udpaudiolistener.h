#ifndef UDPAUDIOLISTENER_H
#define UDPAUDIOLISTENER_H

#include <QObject>
#include <QDebug>
#include "udplistenerinterface.h"

class UdpAudioListener : public QObject, public UdpListenerInterface
{
    Q_OBJECT
    Q_INTERFACES(UdpListenerInterface)
public:
    explicit UdpAudioListener(QObject *parent = nullptr);

signals:

};

#endif // UDPAUDIOLISTENER_H
