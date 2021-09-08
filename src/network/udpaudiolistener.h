#ifndef UDPAUDIOLISTENER_H
#define UDPAUDIOLISTENER_H

#include <QObject>
#include <QDebug>

class UdpAudioListener : public QObject
{
    Q_OBJECT
public:
    explicit UdpAudioListener(QObject *parent = nullptr);

signals:

};

#endif // UDPAUDIOLISTENER_H
