#ifndef KIJANGNETWORKMANAGER_H
#define KIJANGNETWORKMANAGER_H

#include <QObject>
#include <QDebug>
#include <QSettings>
#include "kijangtcpserver.h"
#include "udpaudiolistener.h"
#include "udpvideolistener.h"
#include "udpmotionlistener.h"
#include "udpaudiooutput.h"
#include "udpvideooutput.h"
#include "udpmotionoutput.h"

class KijangNetworkManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint32 commServerPort READ commServerPort WRITE setCommServerPort NOTIFY commServerPortChanged)
    Q_PROPERTY(quint32 statusServerPort READ statusServerPort WRITE setStatusServerPort NOTIFY statusServerPortChanged)
    Q_PROPERTY(bool commServerError READ commServerError WRITE setCommServerError NOTIFY commServerErrorChanged)
    Q_PROPERTY(bool statusServerError READ statusServerError WRITE setStatusServerError NOTIFY statusServerErrorChanged)
    Q_PROPERTY(QString commServerErrorString READ commServerErrorString WRITE setCommServerErrorString NOTIFY commServerErrorStringChanged)
    Q_PROPERTY(QString statusServerErrorString READ statusServerErrorString WRITE setStatusServerErrorString NOTIFY statusServerErrorStringChanged)
public:
    explicit KijangNetworkManager(QObject *parent = nullptr);
    ~KijangNetworkManager();
    void start();
    void stop();

    quint32 commServerPort() const;
    void setCommServerPort(quint32 newCommServerPort);
    quint32 statusServerPort() const;
    void setStatusServerPort(quint32 newStatusServerPort);
    bool commServerError() const;
    void setCommServerError(bool newCommServerError);
    bool statusServerError() const;
    void setStatusServerError(bool newStatusServerError);
    const QString &commServerErrorString() const;
    void setCommServerErrorString(const QString &newCommServerErrorString);
    const QString &statusServerErrorString() const;
    void setStatusServerErrorString(const QString &newStatusServerErrorString);

signals:
    void commServerPortChanged();
    void statusServerPortChanged();
    void commServerErrorChanged();
    void statusServerErrorChanged();
    void commServerErrorStringChanged();
    void statusServerErrorStringChanged();

private:
    quint32 m_commServerPort;
    quint32 m_statusServerPort;
    bool m_commServerError;
    bool m_statusServerError;
    QString m_commServerErrorString;
    QString m_statusServerErrorString;

    KijangTcpServer commServer;
    KijangTcpServer statusServer;
    QList<UdpAudioListener> audioListenerList;
    QList<UdpVideoListener> videoListenerList;
    QList<UdpMotionListener> motionListenerList;
    QList<UdpAudioOutput> audioOutputList;
    QList<UdpVideoOutput> videoOutputList;
    QList<UdpMotionOutput> motionOutputList;
};

#endif // KIJANGNETWORKMANAGER_H
