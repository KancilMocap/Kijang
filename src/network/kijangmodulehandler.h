#ifndef KIJANGMODULEHANDLER_H
#define KIJANGMODULEHANDLER_H

#include <QtPlugin>
#include "../../inc/kijangProtocol/kijangprotocol.h"

class KijangModuleHandler
{
    Q_DISABLE_COPY(KijangModuleHandler)
public:
    explicit KijangModuleHandler() = default;
    virtual quint16 module() = 0;

    // Handle request from the client
    virtual void handleRequest(Kijang::KijangProtocol req) = 0;
    // Handle requests from other modules
    virtual void handleLocalRequest(quint16 src, Kijang::KijangProtocol res) = 0;
    // Whether this module can handle the specific code
    virtual bool canHandleCode(quint16 code) = 0;
    // Slot - whether a module is present
    virtual void modulePresent(quint16 module, bool present) = 0;
    // Slot - Whether the module can handle the selected code
    virtual void codePresent(quint16 module, quint16 code, bool present) = 0;

signals:
    // Send packet to a specific client on the server
    virtual void sendResponse(Kijang::KijangProtocol req) = 0;
    // Send packet to a local module handler
    virtual void sendLocalResponse(quint16 src, quint16 target, Kijang::KijangProtocol req) = 0;
    // Whether a specific module is present
    virtual void checkModulePresent(quint16 src, quint16 module) = 0;
    // Whether a a specific module can handle a specific code
    virtual void checkCodePresent(quint16 src, quint16 module, quint16 code) = 0;

};

Q_DECLARE_INTERFACE(KijangModuleHandler, "moe.kancil.kijang.network.modulehandler");

#endif // KIJANGMODULEHANDLER_H
