#ifndef UDPMOTIONOUTPUT_H
#define UDPMOTIONOUTPUT_H

#include <QObject>
#include <QDebug>

class UdpMotionOutput : public QObject
{
    Q_OBJECT
public:
    explicit UdpMotionOutput(QObject *parent = nullptr);

signals:

};

#endif // UDPMOTIONOUTPUT_H
