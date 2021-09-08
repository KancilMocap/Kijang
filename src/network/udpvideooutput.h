#ifndef UDPVIDEOOUTPUT_H
#define UDPVIDEOOUTPUT_H

#include <QObject>
#include <QDebug>

class UdpVideoOutput : public QObject
{
    Q_OBJECT
public:
    explicit UdpVideoOutput(QObject *parent = nullptr);

signals:

};

#endif // UDPVIDEOOUTPUT_H
