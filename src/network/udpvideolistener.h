#ifndef UDPVIDEOLISTENER_H
#define UDPVIDEOLISTENER_H

#include <QObject>
#include <QDebug>

class UdpVideoListener : public QObject
{
    Q_OBJECT
public:
    explicit UdpVideoListener(QObject *parent = nullptr);

signals:

};

#endif // UDPVIDEOLISTENER_H
