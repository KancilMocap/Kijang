#include "communicationclient.h"

int CommunicationClient::INITIAL_TIMEOUT = 3000;

CommunicationClient::CommunicationClient(QObject *parent, qintptr handle) : QObject(parent)
{
    QLoggingCategory network("network");
    qDebug(network) << "Communication client with handle" << handle << "constructed";
    this->handle = handle;
}

CommunicationClient::~CommunicationClient()
{
    QLoggingCategory network("network");
    qDebug(network) << "Communication client with handle" << handle << "deconstructed";
}


void CommunicationClient::run()
{

}
