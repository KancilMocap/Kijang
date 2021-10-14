#ifndef UDPLISTENERINTERFACE_H
#define UDPLISTENERINTERFACE_H

#include <QtPlugin>

class UdpListenerInterface
{
    Q_DISABLE_COPY(UdpListenerInterface)
public:
    explicit UdpListenerInterface() = default;

};

Q_DECLARE_INTERFACE(UdpListenerInterface, "moe.kancil.kijang.network.udplistenerinterface")

#endif // UDPLISTENERINTERFACE_H
