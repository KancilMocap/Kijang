#ifndef KIJANGNETWORKMANAGER_H
#define KIJANGNETWORKMANAGER_H

#include <QObject>
#include <QDebug>
#include <QSettings>
#include <QDir>
#include "kijangmodulehandler.h"
#include "kijangmodule.h"
#include "servercontrolmodule.h"
#include "kijangreadserver.h"
#include "kijangwriteserver.h"
#include "kijangstatusserver.h"
#include "udpaudiolistener.h"
#include "udpvideolistener.h"
#include "udpmotionlistener.h"
#include "udpaudiooutput.h"
#include "udpvideooutput.h"
#include "udpmotionoutput.h"

class KijangNetworkManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint16 readServerPort READ readServerPort WRITE setReadServerPort NOTIFY readServerPortChanged)
    Q_PROPERTY(quint16 writeServerPort READ writeServerPort WRITE setWriteServerPort NOTIFY writeServerPortChanged)
    Q_PROPERTY(quint16 statusServerPort READ statusServerPort WRITE setStatusServerPort NOTIFY statusServerPortChanged)
    Q_PROPERTY(bool readServerError READ readServerError WRITE setReadServerError NOTIFY readServerErrorChanged)
    Q_PROPERTY(bool writeServerError READ writeServerError WRITE setWriteServerError NOTIFY writeServerErrorChanged)
    Q_PROPERTY(bool statusServerError READ statusServerError WRITE setStatusServerError NOTIFY statusServerErrorChanged)
    Q_PROPERTY(QString readServerErrorString READ readServerErrorString WRITE setReadServerErrorString NOTIFY readServerErrorStringChanged)
    Q_PROPERTY(QString writeServerErrorString READ writeServerErrorString WRITE setWriteServerErrorString NOTIFY writeServerErrorStringChanged)
    Q_PROPERTY(QString statusServerErrorString READ statusServerErrorString WRITE setStatusServerErrorString NOTIFY statusServerErrorStringChanged)
public:
    explicit KijangNetworkManager(QObject *parent = nullptr);
    ~KijangNetworkManager();
    void start();
    void stop();

    quint16 readServerPort() const;
    void setReadServerPort(quint16 newReadServerPort);
    quint16 statusServerPort() const;
    void setStatusServerPort(quint16 newStatusServerPort);
    bool readServerError() const;
    void setReadServerError(bool newReadServerError);
    bool statusServerError() const;
    void setStatusServerError(bool newStatusServerError);
    const QString &readServerErrorString() const;
    void setReadServerErrorString(const QString &newReadServerErrorString);
    const QString &statusServerErrorString() const;
    void setStatusServerErrorString(const QString &newStatusServerErrorString);

    Q_INVOKABLE void statusAutodetect();
    Q_INVOKABLE void readAutoDetect();
    Q_INVOKABLE void writeAutoDetect();

    void addModule(KijangModuleHandler *module);
    void removeModule(quint16 module);

    quint16 writeServerPort() const;
    void setWriteServerPort(quint16 newWriteServerPort);
    bool writeServerError() const;
    void setWriteServerError(bool newWriteServerError);
    const QString &writeServerErrorString() const;
    void setWriteServerErrorString(const QString &newWriteServerErrorString);

signals:
    void readServerPortChanged();
    void statusServerPortChanged();
    void readServerErrorChanged();
    void statusServerErrorChanged();
    void readServerErrorStringChanged();
    void statusServerErrorStringChanged();
    void sendResponse(quint32 clientID, KijangProtocol response);
    void writeServerPortChanged();
    void writeServerErrorChanged();
    void writeServerErrorStringChanged();

public slots:
    void handleRequest(quint32 clientID, KijangProtocol request);
    void readClientTerminated(quint32 clientID);
    void readClientAdded(quint32 clientID);
    void writeClientTerminated(quint32 clientID);
    void writeVerifyClient(qintptr handle, quint32 clientID);

    // Received from module handlers
    void moduleSendResponse(KijangProtocol req);
    void sendLocalResponse(quint16 src, quint16 target, KijangProtocol req);
    void checkModulePresent(quint16 src, quint16 module);
    void checkCodePresent(quint16 src, quint16 module, quint16 code);

private:
    bool sourceModuleExists(quint16 module);
    void addModuleAfterCheck(KijangModuleHandler *module);
    void resetReadServer();
    void resetWriteServer();
    void resetStatusServer();

    KijangModule module7FFE;
    ServerControlModule module7FFF;

    static QString settingsFile;
    quint16 m_readServerPort;
    quint16 m_writeServerPort;
    quint16 m_statusServerPort;
    bool m_readServerError;
    bool m_writeServerError;
    bool m_statusServerError;
    QString m_readServerErrorString;
    QString m_writeServerErrorString;
    QString m_statusServerErrorString;

    KijangReadServer *readServer = nullptr;
    KijangWriteServer *writeServer = nullptr;
    KijangStatusServer *statusServer = nullptr;
    QList<UdpAudioListener> audioListenerList;
    QList<UdpVideoListener> videoListenerList;
    QList<UdpMotionListener> motionListenerList;
    QList<UdpAudioOutput> audioOutputList;
    QList<UdpVideoOutput> videoOutputList;
    QList<UdpMotionOutput> motionOutputList;
    QList<quint32> clientList;
    QMap<quint16, KijangModuleHandler *> moduleHandlerList;
};

#endif // KIJANGNETWORKMANAGER_H
