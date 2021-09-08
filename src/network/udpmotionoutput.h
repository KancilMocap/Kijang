#ifndef UDPMOTIONOUTPUT_H
#define UDPMOTIONOUTPUT_H

#include <QObject>
#include <QDebug>
#include "udpoutputinterface.h"

class UdpMotionOutput : public QObject, public UdpOutputInterface
{
    Q_OBJECT
public:
    explicit UdpMotionOutput(QObject *parent = nullptr);

signals:

};

#endif // UDPMOTIONOUTPUT_H
