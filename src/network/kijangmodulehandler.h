#ifndef KIJANGMODULEHANDLER_H
#define KIJANGMODULEHANDLER_H

#include <QtPlugin>
#include "kijangprotocol.h"

class KijangModuleHandler
{
    Q_DISABLE_COPY(KijangModuleHandler)
public:
    explicit KijangModuleHandler() = default;
    virtual ~KijangModuleHandler() {}
    virtual quint16 module() = 0;
    // Returns a list of request codes and their corresponding descriptions
    virtual QMap<quint16, QString> authBlocks() = 0;

    // Handle request from the client
    virtual void handleRequest(KijangProtocol req) = 0;
    // Handle requests from other modules
    virtual void handleLocalRequest(quint16 src, KijangProtocol req) = 0;
    // Whether this module can handle the specific code
    virtual bool canHandleCode(quint16 code) = 0;
    // Slot - whether a module is present
    virtual void modulePresent(quint16 module, bool present) = 0;
    // Slot - Whether the module can handle the selected code
    virtual void codePresent(quint16 module, quint16 code, bool present) = 0;

signals:
    // Send packet to a specific client on the server
    virtual void sendResponse(KijangProtocol req) = 0;
    // Send packet to a local module handler
    virtual void sendLocalResponse(quint16 src, quint16 target, KijangProtocol req) = 0;
    // Whether a specific module is present
    virtual void checkModulePresent(quint16 src, quint16 module) = 0;
    // Whether a a specific module can handle a specific code
    virtual void checkCodePresent(quint16 src, quint16 module, quint16 code) = 0;

};

Q_DECLARE_INTERFACE(KijangModuleHandler, "moe.kancil.kijang.network.modulehandler");

#endif // KIJANGMODULEHANDLER_H
