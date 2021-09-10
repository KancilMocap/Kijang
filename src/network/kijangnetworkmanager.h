#ifndef KIJANGNETWORKMANAGER_H
#define KIJANGNETWORKMANAGER_H

#include <QObject>
#include <QDebug>
#include "kijangtcpserver.h"
#include "udpaudiolistener.h"
#include "udpvideolistener.h"
#include "udpmotionlistener.h"
#include "udpaudiooutput.h"
#include "udpvideooutput.h"
#include "udpmotionoutput.h"

class KijangNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit KijangNetworkManager(QObject *parent = nullptr);

signals:

private:
    KijangTcpServer commServer;
    KijangTcpServer statusServer;
    QList<UdpAudioListener> audioListenerList;
    QList<UdpVideoListener> videoListenerList;
    QList<UdpMotionListener> motionListenerList;
    QList<UdpAudioOutput> audioOutputList;
    QList<UdpVideoOutput> videoOutputList;
    QList<UdpMotionOutput> motionOutputList;
};

#endif // KIJANGNETWORKMANAGER_H
