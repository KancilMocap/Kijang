#ifndef UDPMOTIONLISTENER_H
#define UDPMOTIONLISTENER_H

#include <QObject>
#include <QDebug>

class UdpMotionListener : public QObject
{
    Q_OBJECT
public:
    explicit UdpMotionListener(QObject *parent = nullptr);

signals:

};

#endif // UDPMOTIONLISTENER_H
