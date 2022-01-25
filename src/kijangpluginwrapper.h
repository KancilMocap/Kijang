#ifndef KIJANGPLUGINWRAPPER_H
#define KIJANGPLUGINWRAPPER_H

#include <QObject>
#include "input/audioinput.h"
#include "input/videoinput.h"
#include "input/motioninput.h"
#include "network/udplistenerinterface.h"
#include "network/kijangmodulehandler.h"
#include "kijangplugin.h"

class KijangPluginWrapper : public QObject
{
    Q_OBJECT
public:
    explicit KijangPluginWrapper(KijangPlugin *pluginObject, QObject *parent = nullptr);

private:
    void connectPluginSignals(KijangPlugin *pluginObject);
    QString pluginID;

signals:
    // Events - Signals
    void audioInputAdded(QString src, AudioInput *input);
    void audioInputRemoved(QString src, AudioInput *input);
    void videoInputAdded(QString src, VideoInput *input);
    void videoInputRemoved(QString src, VideoInput *input);
    void motionInputAdded(QString src, MotionInput *input);
    void motionInputRemoved(QString src, MotionInput *input);

    void udpListenerInterfaceAdded(QString src, UdpListenerInterface *interface);
    void udpListenerInterfaceRemoved(QString src, UdpListenerInterface *interface);
    void moduleHandlerAdded(QString src, KijangModuleHandler *handler);
    void moduleHandlerRemoved(QString src, KijangModuleHandler *handler);

    // Request all plugins - Signals
    void requestAllMetadata(QString src);
    void requestAllPlugins(QString src);

    void requestAllAudioInput(QString src);
    void requestAllMotionInput(QString src);
    void requestAllVideoInput(QString src);

    void requestAllUdpListener(QString src);
    void requestAllModuleHandlers(QString src);

    // Request specific plugin  - Signals
    void requestPluginMetadata(QString src, QString plugin);
    void requestPluginObject(QString src, QString object);

    void requestPluginAudioInput(QString src, QString plugin);
    void requestPluginMotionInput(QString src, QString plugin);
    void requestPluginVideoInput(QString src, QString plugin);

    void requestPluginUdpListener(QString src, QString plugin);
    void requestPluginModuleHandlers(QString src, QString plugin);

    // Special events
    void eventSignal(QString src, QList<QVariant> values);
    void fatalSignal(QString src, QString title, QList<QVariant> values);

public slots:
    // Events - Signals
    void forwardAudioInputAdded(AudioInput *input);
    void forwardAudioInputRemoved(AudioInput *input);
    void forwardVideoInputAdded(VideoInput *input);
    void forwardVideoInputRemoved(VideoInput *input);
    void forwardMotionInputAdded(MotionInput *input);
    void forwardMotionInputRemoved(MotionInput *input);

    void forwardUdpListenerInterfaceAdded(UdpListenerInterface *interface);
    void forwardUdpListenerInterfaceRemoved(UdpListenerInterface *interface);
    void forwardModuleHandlerAdded(KijangModuleHandler *handler);
    void forwardModuleHandlerRemoved(KijangModuleHandler *handler);

    // Request all plugins - Signals
    void forwardRequestAllMetadata();
    void forwardRequestAllPlugins();

    void forwardRequestAllAudioInput();
    void forwardRequestAllMotionInput();
    void forwardRequestAllVideoInput();

    void forwardRequestAllUdpListener();
    void forwardRequestAllModuleHandlers();

    // Request specific plugin  - Signals
    void forwardRequestPluginMetadata(QString plugin);
    void forwardRequestPluginObject(QString object);

    void forwardRequestPluginAudioInput(QString plugin);
    void forwardRequestPluginMotionInput(QString plugin);
    void forwardRequestPluginVideoInput(QString plugin);

    void forwardRequestPluginUdpListener(QString plugin);
    void forwardRequestPluginModuleHandlers(QString plugin);

    // Special events
    void forwardEventSignal(QList<QVariant> values);
    void forwardFatalSignal(QString title, QList<QVariant> values);
};

#endif // KIJANGPLUGINWRAPPER_H
