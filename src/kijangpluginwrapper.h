#ifndef KIJANGPLUGINWRAPPER_H
#define KIJANGPLUGINWRAPPER_H

#include <QObject>
#include "input/audioinput.h"
#include "input/motioninput.h"
#include "input/videoinput.h"
#include "network/udplistenerinterface.h"
#include "network/kijangmodulehandler.h"

class KijangPluginWrapper : public QObject
{
    Q_OBJECT
public:
    explicit KijangPluginWrapper(QObject *parent = nullptr);

    void setPlugin(QString plugin);

signals:
    // Forwarded signals from plugin to server
    // Events - Signals (KijangPlugin)
    void forwardAudioInputAdded(QString src, AudioInput *input);
    void forwardAudioInputRemoved(QString src, AudioInput *input);
    void forwardVideoInputAdded(QString src, VideoInput *input);
    void forwardVideoInputRemoved(QString src, VideoInput *input);
    void forwardMotionInputAdded(QString src, MotionInput *input);
    void forwardMotionInputRemoved(QString src, MotionInput *input);

    void forwardUdpListenerInterfaceAdded(QString src, UdpListenerInterface *interface);
    void forwardUdpListenerInterfaceRemoved(QString src, UdpListenerInterface *interface);
    void forwardModuleHandlerAdded(QString src, KijangModuleHandler *handler);
    void forwardModuleHandlerRemoved(QString src, KijangModuleHandler *handler);

    // Request all plugins - Signals (QString src, KijangPlugin)
    void forwardRequestAllMetadata(QString src);
    void forwardRequestAllPlugins(QString src);

    void forwardRequestAllAudioInput(QString src);
    void forwardRequestAllMotionInput(QString src);
    void forwardRequestAllVideoInput(QString src);

    void forwardRequestAllUdpListener(QString src);
    void forwardRequestAllModuleHandlers(QString src);

    // Request specific plugin  - Signals (QString src, KijangPlugin)
    void forwardRequestPluginMetadata(QString src, QString plugin);
    void forwardRequestPluginObject(QString src, QString object);

    void forwardRequestPluginAudioInput(QString src, QString plugin);
    void forwardRequestPluginMotionInput(QString src, QString plugin);
    void forwardRequestPluginVideoInput(QString src, QString plugin);

    void forwardRequestPluginUdpListener(QString src, QString plugin);
    void forwardRequestPluginModuleHandlers(QString src, QString plugin);

    // Special events (QString src, KijangPlugin)
    void forwardEventSignal(QString src, QList<QVariant> values);
    void forwardFatalSignal(QString src, QString error=nullptr);

public slots:
    // Forwarded signals from plugin to server
    // Events - Signals (KijangPlugin)
    void audioInputAdded(AudioInput *input);
    void audioInputRemoved(AudioInput *input);
    void videoInputAdded(VideoInput *input);
    void videoInputRemoved(VideoInput *input);
    void motionInputAdded(MotionInput *input);
    void motionInputRemoved(MotionInput *input);

    void udpListenerInterfaceAdded(UdpListenerInterface *interface);
    void udpListenerInterfaceRemoved(UdpListenerInterface *interface);
    void moduleHandlerAdded(KijangModuleHandler *handler);
    void moduleHandlerRemoved(KijangModuleHandler *handler);

    // Request all plugins - Signals (KijangPlugin)
    void requestAllMetadata();
    void requestAllPlugins();

    void requestAllAudioInput();
    void requestAllMotionInput();
    void requestAllVideoInput();

    void requestAllUdpListener();
    void requestAllModuleHandlers();

    // Request specific plugin  - Signals (KijangPlugin)
    void requestPluginMetadata(QString plugin);
    void requestPluginObject(QString plugin);

    void requestPluginAudioInput(QString plugin);
    void requestPluginMotionInput(QString plugin);
    void requestPluginVideoInput(QString plugin);

    void requestPluginUdpListener(QString plugin);
    void requestPluginModuleHandlers(QString plugin);

    // Special events (KijangPlugin)
    void eventSignal(QList<QVariant> values);
    void fatalSignal(QString error=nullptr);

private:
    QString pluginID;

};

#endif // KIJANGPLUGINWRAPPER_H
