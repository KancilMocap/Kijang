#ifndef SERVERCONTROLMODULE_H
#define SERVERCONTROLMODULE_H

#include <QObject>
#include <QLoggingCategory>
#include "kijangmodulehandler.h"

class ServerControlModule : public QObject, public KijangModuleHandler
{
    Q_OBJECT
    Q_INTERFACES(KijangModuleHandler)
public:
    explicit ServerControlModule(QObject *parent = nullptr);

    // KijangModuleHandler interface
public:
    quint16 module() Q_DECL_OVERRIDE;
    void handleRequest(KijangProtocol req) Q_DECL_OVERRIDE;
    void handleLocalRequest(quint16 src, KijangProtocol req) Q_DECL_OVERRIDE;
    bool canHandleCode(quint16 code) Q_DECL_OVERRIDE;
    void modulePresent(quint16 module, bool present) Q_DECL_OVERRIDE;
    void codePresent(quint16 module, quint16 code, bool present) Q_DECL_OVERRIDE;

private:
    void handleInternal(bool isClient, quint16 src, KijangProtocol req);
    QMap<quint16, QString> m_authBlocks;

signals:
    void sendResponse(KijangProtocol req) Q_DECL_OVERRIDE;
    void sendLocalResponse(quint16 src, quint16 target, KijangProtocol req) Q_DECL_OVERRIDE;
    void checkModulePresent(quint16 src, quint16 module) Q_DECL_OVERRIDE;
    void checkCodePresent(quint16 src, quint16 module, quint16 code) Q_DECL_OVERRIDE;

    // KijangModuleHandler interface
public:
    QMap<quint16, QString> authBlocks() Q_DECL_OVERRIDE;
};

#endif // SERVERCONTROLMODULE_H
