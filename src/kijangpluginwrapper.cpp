#include "kijangpluginwrapper.h"

KijangPluginWrapper::KijangPluginWrapper(KijangPlugin *pluginObject, QObject *parent)
    : QObject{parent}
{
    pluginID = pluginObject->metadata().pluginID;
    connectPluginSignals(pluginObject);
}

void KijangPluginWrapper::connectPluginSignals(KijangPlugin *pluginObject)
{
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(audioInputAdded(AudioInput*)), this, SLOT(forwardAudioInputAdded(AudioInput*)));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(audioInputRemoved(AudioInput*)), this, SLOT(forwardAudioInputRemoved(AudioInput*)));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(videoInputAdded(VideoInput*)), this, SLOT(forwardVideoInputAdded(VideoInput*)));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(videoInputRemoved(VideoInput*)), this, SLOT(forwardVideoInputRemoved(VideoInput*)));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(motionInputAdded(MotionInput*)), this, SLOT(forwardMotionInputAdded(MotionInput*)));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(motionInputRemoved(MotionInput*)), this, SLOT(forwardMotionInputRemoved(MotionInput*)));

    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(udpListenerInterfaceAdded(UdpListenerInterface*)), this, SLOT(forwardUdpListenerInterfaceAdded(UdpListenerInterface*)));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(udpListenerInterfaceRemoved(UdpListenerInterface*)), this, SLOT(forwardUdpListenerInterfaceRemoved(UdpListenerInterface*)));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(moduleHandlerAdded(KijangModuleHandler*)), this, SLOT(forwardModuleHandlerAdded(KijangModuleHandler*)));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(moduleHandlerRemoved(KijangModuleHandler*)), this, SLOT(forwardModuleHandlerRemoved(KijangModuleHandler*)));

    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(requestAllMetadata()), this, SLOT(forwardRequestAllMetadata()));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(requestAllPlugins()), this, SLOT(forwardRequestAllPlugins()));

    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(requestAllAudioInput()), this, SLOT(forwardRequestAllAudioInput()));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(requestAllMotionInput()), this, SLOT(forwardRequestAllMotionInput()));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(requestAllVideoInput()), this, SLOT(forwardRequestAllVideoInput()));

    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(requestAllUdpListener()), this, SLOT(forwardRequestAllUdpListener()));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(requestAllModuleHandlers()), this, SLOT(forwardRequestAllModuleHandlers()));

    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(requestPluginMetadata()), this, SLOT(forwardRequestPluginMetadata()));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(requestPluginObject()), this, SLOT(forwardRequestPluginObject()));

    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(requestPluginAudioInput()), this, SLOT(forwardRequestPluginAudioInput()));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(requestPluginMotionInput()), this, SLOT(forwardRequestPluginMotionInput()));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(requestPluginVideoInput()), this, SLOT(forwardRequestPluginVideoInput()));

    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(requestPluginUdpListener()), this, SLOT(forwardRequestPluginUdpListener()));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(requestPluginModuleHandlers()), this, SLOT(forwardRequestPluginModuleHandlers()));

    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(eventSignal(QList<QVariant>)), this, SLOT(forwardEventSignal(QList<QVariant>)));
    connect(dynamic_cast<QObject *>(pluginObject), SIGNAL(fatalSignal(QString,QList<QVariant>)), this, SLOT(forwardFatalSignal(QString,QList<QVariant>)));
}

void KijangPluginWrapper::forwardAudioInputAdded(AudioInput *input)
{
    emit audioInputAdded(pluginID, input);
}

void KijangPluginWrapper::forwardAudioInputRemoved(AudioInput *input)
{
    emit audioInputRemoved(pluginID, input);
}

void KijangPluginWrapper::forwardVideoInputAdded(VideoInput *input)
{
    emit videoInputAdded(pluginID, input);
}

void KijangPluginWrapper::forwardVideoInputRemoved(VideoInput *input)
{
    emit videoInputRemoved(pluginID, input);
}

void KijangPluginWrapper::forwardMotionInputAdded(MotionInput *input)
{
    emit motionInputAdded(pluginID, input);
}

void KijangPluginWrapper::forwardMotionInputRemoved(MotionInput *input)
{
    emit motionInputRemoved(pluginID, input);
}

void KijangPluginWrapper::forwardUdpListenerInterfaceAdded(UdpListenerInterface *interface)
{
    emit udpListenerInterfaceAdded(pluginID, interface);
}

void KijangPluginWrapper::forwardUdpListenerInterfaceRemoved(UdpListenerInterface *interface)
{
    emit udpListenerInterfaceRemoved(pluginID, interface);
}

void KijangPluginWrapper::forwardModuleHandlerAdded(KijangModuleHandler *handler)
{
    emit moduleHandlerAdded(pluginID, handler);
}

void KijangPluginWrapper::forwardModuleHandlerRemoved(KijangModuleHandler *handler)
{
    emit moduleHandlerRemoved(pluginID, handler);
}

void KijangPluginWrapper::forwardRequestAllMetadata()
{
    emit requestAllMetadata(pluginID);
}

void KijangPluginWrapper::forwardRequestAllPlugins()
{
    emit requestAllPlugins(pluginID);
}

void KijangPluginWrapper::forwardRequestAllAudioInput()
{
    emit requestAllAudioInput(pluginID);
}

void KijangPluginWrapper::forwardRequestAllMotionInput()
{
    emit requestAllMotionInput(pluginID);
}

void KijangPluginWrapper::forwardRequestAllVideoInput()
{
    emit requestAllVideoInput(pluginID);
}

void KijangPluginWrapper::forwardRequestAllUdpListener()
{
    emit requestAllUdpListener(pluginID);
}

void KijangPluginWrapper::forwardRequestAllModuleHandlers()
{
    emit requestAllModuleHandlers(pluginID);
}

void KijangPluginWrapper::forwardRequestPluginMetadata(QString plugin)
{
    emit requestPluginMetadata(pluginID, plugin);
}

void KijangPluginWrapper::forwardRequestPluginObject(QString object)
{
    emit requestPluginObject(pluginID, object);
}

void KijangPluginWrapper::forwardRequestPluginAudioInput(QString plugin)
{
    emit requestPluginAudioInput(pluginID, plugin);
}

void KijangPluginWrapper::forwardRequestPluginMotionInput(QString plugin)
{
    emit requestPluginMotionInput(pluginID, plugin);
}

void KijangPluginWrapper::forwardRequestPluginVideoInput(QString plugin)
{
    emit requestPluginVideoInput(pluginID, plugin);
}

void KijangPluginWrapper::forwardRequestPluginUdpListener(QString plugin)
{
    emit requestPluginUdpListener(pluginID, plugin);
}

void KijangPluginWrapper::forwardRequestPluginModuleHandlers(QString plugin)
{
    emit requestPluginModuleHandlers(pluginID, plugin);
}

void KijangPluginWrapper::forwardEventSignal(QList<QVariant> values)
{
    emit eventSignal(pluginID, values);
}

void KijangPluginWrapper::forwardFatalSignal(QString title, QList<QVariant> values)
{
    emit fatalSignal(pluginID, title, values);
}
