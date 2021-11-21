#include "servercontrolmodule.h"

Q_DECLARE_LOGGING_CATEGORY(serverControl);
Q_LOGGING_CATEGORY(serverControl,"serverModule-7FFF");

ServerControlModule::ServerControlModule(QObject *parent) : QObject(parent)
{

}

quint16 ServerControlModule::module()
{
    return 32767; // 7FFF
}

void ServerControlModule::handleRequest(KijangProtocol req)
{
    handleInternal(true, 0, req);
}

void ServerControlModule::handleLocalRequest(quint16 src, KijangProtocol req)
{
    handleInternal(false, src, req);
}

bool ServerControlModule::canHandleCode(quint16 code)
{
    // 0000-0005, 000F-0017, 0020-0023
    return (code >= 0 && code <= 5) || (code >= 15 && code <= 23) || (code >= 32 && code <= 35);
}

void ServerControlModule::modulePresent(quint16 module, bool present)
{

}

void ServerControlModule::codePresent(quint16 module, quint16 code, bool present)
{

}

void ServerControlModule::handleInternal(bool isClient, quint16 src, KijangProtocol req)
{
    // TODO: Complete
    KijangProtocol res(req.clientID(), 32767, req.code());
    res.setRequestID(req.requestID());
    qDebug(serverControl) << "Handling client" << req.clientID() << "with code" << req.code();
    switch (req.code()) {
    case 0: // 0000
        if (isClient) qInfo(serverControl) << "Generic message received from client" << req.clientID() << ": " << QString(req.data()).toUtf8();
        else qInfo(serverControl) << "Generic message received from module" << src << ": " << QString(req.data()).toUtf8();
        break;
    case 1: // 0001
        if (isClient) emit sendResponse(res); // Only mirrors response
        else qWarning(serverControl) << "Code 0001 is reserved for clients only (from " << src << ")";
        break;
    case 2: // 0002
        if (isClient) emit sendResponse(res); // Only mirrors response
        else qWarning(serverControl) << "Code 0002 is reserved for clients only (from " << src << ")";
        break;
    case 3: // 0003
        break;
    case 4: // 0004
        break;
    case 5: // 0005
        break;
    case 15: // 000F
        break;
    case 16: // 0010
        break;
    case 17: // 0011
        break;
    case 18: // 0012
        break;
    case 19: // 0013
        break;
    case 20: // 0014
        break;
    case 21: // 0015
        break;
    case 22: // 0016
        break;
    case 23: // 0017
        break;
    case 32: // 0020
        break;
    case 33: // 0021
        break;
    case 34: // 0022
        break;
    case 35: // 0023
        break;
    default:
        qWarning(serverControl) << "Invalid code" << req.code() << "received";
        break;
    }
}

QMap<quint16, QString> ServerControlModule::authBlocks()
{
    return m_authBlocks;
}
