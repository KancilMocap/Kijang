#ifndef UDPVIDEOOUTPUT_H
#define UDPVIDEOOUTPUT_H

#include <QObject>
#include <QDebug>
#include "udpoutputinterface.h"

class UdpVideoOutput : public QObject, public UdpOutputInterface
{
    Q_OBJECT
public:
    explicit UdpVideoOutput(QObject *parent = nullptr);

signals:

};

#endif // UDPVIDEOOUTPUT_H
