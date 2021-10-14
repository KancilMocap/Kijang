#include "kijangpluginwrapper.h"

KijangPluginWrapper::KijangPluginWrapper(QObject *parent) : QObject(parent)
{

}

void KijangPluginWrapper::setPlugin(QString plugin)
{
    pluginID = plugin;
}

void KijangPluginWrapper::audioInputAdded(AudioInput *input)
{
    emit forwardAudioInputAdded(pluginID, input);
}

void KijangPluginWrapper::audioInputRemoved(AudioInput *input)
{
    emit forwardAudioInputRemoved(pluginID, input);
}

void KijangPluginWrapper::videoInputAdded(VideoInput *input)
{
    emit forwardVideoInputAdded(pluginID, input);
}

void KijangPluginWrapper::videoInputRemoved(VideoInput *input)
{
    emit forwardVideoInputRemoved(pluginID, input);
}

void KijangPluginWrapper::motionInputAdded(MotionInput *input)
{
    emit forwardMotionInputAdded(pluginID, input);
}

void KijangPluginWrapper::motionInputRemoved(MotionInput *input)
{
    emit forwardMotionInputRemoved(pluginID, input);
}

void KijangPluginWrapper::udpListenerInterfaceAdded(UdpListenerInterface *interface)
{
    emit forwardUdpListenerInterfaceAdded(pluginID, interface);
}

void KijangPluginWrapper::udpListenerInterfaceRemoved(UdpListenerInterface *interface)
{
    emit forwardUdpListenerInterfaceRemoved(pluginID, interface);
}

void KijangPluginWrapper::moduleHandlerAdded(KijangModuleHandler *handler)
{
    emit forwardModuleHandlerAdded(pluginID, handler);
}

void KijangPluginWrapper::moduleHandlerRemoved(KijangModuleHandler *handler)
{
    emit forwardModuleHandlerRemoved(pluginID, handler);
}

void KijangPluginWrapper::requestAllMetadata()
{
    emit forwardRequestAllMetadata(pluginID);
}

void KijangPluginWrapper::requestAllPlugins()
{
    emit forwardRequestAllPlugins(pluginID);
}

void KijangPluginWrapper::requestAllAudioInput()
{
    emit forwardRequestAllAudioInput(pluginID);
}

void KijangPluginWrapper::requestAllMotionInput()
{
    emit forwardRequestAllMotionInput(pluginID);
}

void KijangPluginWrapper::requestAllVideoInput()
{
    emit forwardRequestAllVideoInput(pluginID);
}

void KijangPluginWrapper::requestAllUdpListener()
{
    emit forwardRequestAllUdpListener(pluginID);
}

void KijangPluginWrapper::requestAllModuleHandlers()
{
    emit forwardRequestAllModuleHandlers(pluginID);
}

void KijangPluginWrapper::requestPluginMetadata(QString plugin)
{
    emit forwardRequestPluginMetadata(pluginID, plugin);
}

void KijangPluginWrapper::requestPluginObject(QString plugin)
{
    emit forwardRequestPluginObject(pluginID, plugin);
}

void KijangPluginWrapper::requestPluginAudioInput(QString plugin)
{
    emit forwardRequestPluginAudioInput(pluginID, plugin);
}

void KijangPluginWrapper::requestPluginMotionInput(QString plugin)
{
    emit forwardRequestPluginMotionInput(pluginID, plugin);
}

void KijangPluginWrapper::requestPluginVideoInput(QString plugin)
{
    emit forwardRequestPluginVideoInput(pluginID, plugin);
}

void KijangPluginWrapper::requestPluginUdpListener(QString plugin)
{
    emit forwardRequestPluginUdpListener(pluginID, plugin);
}

void KijangPluginWrapper::requestPluginModuleHandlers(QString plugin)
{
    emit forwardRequestPluginModuleHandlers(pluginID, plugin);
}

void KijangPluginWrapper::eventSignal(QList<QVariant> values)
{
    emit forwardEventSignal(pluginID, values);
}

void KijangPluginWrapper::fatalSignal(QString error)
{
    emit forwardFatalSignal(pluginID, error);
    disconnect(); // The plugin is assumed to be dead after this point, will be deleted by the plugin manager
}
