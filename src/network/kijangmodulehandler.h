#ifndef KIJANGMODULEHANDLER_H
#define KIJANGMODULEHANDLER_H

#include <QtPlugin>
#include "inc/kijangProtocol/kijangprotocol.h"

class KijangModuleHandler
{
public:
    virtual quint16 module() = 0;

    // Handle request from another client / other modules
    virtual Kijang::KijangProtocol handleRequest(Kijang::KijangProtocol req) = 0;
    // Slot - Handle response that is sent via localRequest, clientRequest, allClientsRequest
    virtual Kijang::KijangProtocol handleResponse(Kijang::KijangProtocol res) = 0;
    // Whether the module can handle the specific code
    virtual bool canHandleCode(quint16 code) = 0;
    // Slot - whether a module is present
    virtual void modulePresent(quint16 module, bool presenct) = 0;
    // Slot - Whether the module can handle the selected code
    virtual void codePresent(quint16 module, quint16 code, bool present) = 0;

signals:
    // Sending async response back to client / other modules
    virtual void asyncResponse(Kijang::KijangProtocol res) = 0;
    // Send request to another module on the server
    virtual void localRequest(Kijang::KijangProtocol req) = 0;
    // Send request to a specific client on the server
    virtual void clientRequest(Kijang::KijangProtocol req) = 0;
    // Send request to all clients connected to the server
    virtual void allClientsRequest(Kijang::KijangProtocol req) = 0;
    // Whether a specific module is present
    virtual void checkModulePresent(quint16 module) = 0;
    // Whet a a specific module can handle a specific code
    virtual void checkCodePresent(quint16 module, quint16 code) = 0;

};

Q_DECLARE_INTERFACE(KijangModuleHandler, "moe.kancil.kijang.network.modulehandler");

#endif // KIJANGMODULEHANDLER_H
