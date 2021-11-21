#include "kijangmodule.h"

KijangModule::KijangModule(QObject *parent) : QObject(parent)
{

}

quint16 KijangModule::module()
{
    return 32766; // 7FFE
}

void KijangModule::handleRequest(KijangProtocol req)
{
    handleInternal(true, 0, req);
}

void KijangModule::handleLocalRequest(quint16 src, KijangProtocol req)
{
    handleInternal(false, 0, req);
}

bool KijangModule::canHandleCode(quint16 code)
{
    // 0002-000B, 0010, 0011, 0013-0018, 0020-0025, 0027
    return (code >= 2 && code <= 11) || code == 16 || code == 17 || (code >= 19 && code <= 24) || (code >= 32 && code <= 37) || code == 39;
}

void KijangModule::modulePresent(quint16 module, bool present)
{
}

void KijangModule::codePresent(quint16 module, quint16 code, bool present)
{
}

void KijangModule::handleInternal(bool isClient, quint16 src, KijangProtocol req)
{
    // TODO: Complete
}

QMap<quint16, QString> KijangModule::authBlocks()
{
    return m_authBlocks;
}
