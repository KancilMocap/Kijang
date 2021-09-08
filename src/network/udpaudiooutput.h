#ifndef UDPAUDIOOUTPUT_H
#define UDPAUDIOOUTPUT_H

#include <QObject>
#include <QDebug>

class UdpAudioOutput : public QObject
{
    Q_OBJECT
public:
    explicit UdpAudioOutput(QObject *parent = nullptr);

signals:

};

#endif // UDPAUDIOOUTPUT_H
