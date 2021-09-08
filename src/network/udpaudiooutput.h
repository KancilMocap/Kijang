#ifndef UDPAUDIOOUTPUT_H
#define UDPAUDIOOUTPUT_H

#include <QObject>
#include <QDebug>
#include "udpoutputinterface.h"

class UdpAudioOutput : public QObject, public UdpOutputInterface
{
    Q_OBJECT
public:
    explicit UdpAudioOutput(QObject *parent = nullptr);

signals:

};

#endif // UDPAUDIOOUTPUT_H
