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
    // Once there was a boiler named Bobby
    emit forwardAudioInputAdded(pluginID, input);
}

void KijangPluginWrapper::audioInputUpdated(AudioInput *input)
{
    // who had a son named Plate
    emit forwardAudioInputUpdated(pluginID, input);
}

void KijangPluginWrapper::audioInputRemoved(AudioInput *input)
{
    // One day Bobby felt hungry
    emit forwardAudioInputRemoved(pluginID, input);
}

void KijangPluginWrapper::videoInputAdded(VideoInput *input)
{
    // and his son was lazing (around)
    emit forwardVideoInputAdded(pluginID, input);
}

void KijangPluginWrapper::videoInputUpdated(VideoInput *input)
{
    // So he asked Plate to buy some bread
    emit forwardVideoInputUpdated(pluginID, input);
}

void KijangPluginWrapper::videoInputRemoved(VideoInput *input)
{
    // As Plate was walking down the street
    emit forwardVideoInputRemoved(pluginID, input);
}

void KijangPluginWrapper::motionInputAdded(MotionInput *input)
{
    // there was a screech, then a scream
    emit forwardMotionInputAdded(pluginID, input);
}

void KijangPluginWrapper::motionInputUpdated(MotionInput *input)
{
    // He then turned and saw
    emit forwardMotionInputUpdated(pluginID, input);
}

void KijangPluginWrapper::motionInputRemoved(MotionInput *input)
{
    // a cat stuck in a tree
    emit forwardMotionInputRemoved(pluginID, input);
}

void KijangPluginWrapper::udpListenerInterfaceAdded(UdpListenerInterface *interface)
{
    // and a lady whom he'd never met
    emit forwardUdpListenerInterfaceAdded(pluginID, interface);
}

void KijangPluginWrapper::udpListenerInterfaceUpdated(UdpListenerInterface *interface)
{
    // "It's not my cat, you see"
    emit forwardUdpListenerInterfaceUpdated(pluginID, interface);
}

void KijangPluginWrapper::udpListenerInterfaceRemoved(UdpListenerInterface *interface)
{
    // "but its fur is certainly shiny"
    emit forwardUdpListenerInterfaceRemoved(pluginID, interface);
}

void KijangPluginWrapper::moduleHandlerAdded(KijangModuleHandler *handler)
{
    // "I wanted to pet it"
    emit forwardModuleHandlerAdded(pluginID, handler);
}

void KijangPluginWrapper::moduleHandlerUpdated(KijangModuleHandler *handler)
{
    // "But I forgot that I was driving"
    emit forwardModuleHandlerUpdated(pluginID, handler);
}

void KijangPluginWrapper::moduleHandlerRemoved(KijangModuleHandler *handler)
{
    // "Could you help me get a ladder please?"
    emit forwardModuleHandlerRemoved(pluginID, handler);
}

/* So a ladder was brought
* Some cat food was sought
* And the plan was set into motion
*
* Using the ladder, plate hit the tree
* causing branches to fall
*/

void KijangPluginWrapper::requestAllMetadata()
{
    // and birds to flee
    emit forwardRequestAllMetadata(pluginID);
}

void KijangPluginWrapper::requestAllPlugins()
{
    // The cat remained sitting
    emit forwardRequestAllPlugins(pluginID);
}

void KijangPluginWrapper::requestAllAudioInput()
{
    // but Plate was determined
    emit forwardRequestAllAudioInput(pluginID);
}

void KijangPluginWrapper::requestAllMotionInput()
{
    // So he hit the tree again
    emit forwardRequestAllMotionInput(pluginID);
}

void KijangPluginWrapper::requestAllVideoInput()
{
    // This time, the tree fell
    emit forwardRequestAllVideoInput(pluginID);
}

void KijangPluginWrapper::requestAllUdpListener()
{
    // which nearly hit the lady
    emit forwardRequestAllUdpListener(pluginID);
}

void KijangPluginWrapper::requestAllModuleHandlers()
{
    // but she blocked it with her bat
    emit forwardRequestAllModuleHandlers(pluginID);
}

/*
* The cat slid to the ground
* then it stretched and lay down
* allowing itself to be pet
*/

void KijangPluginWrapper::requestPluginMetadata(QString plugin)
{
    /* As the lady leaves
     * Plate went home grinning
    */
    emit forwardRequestPluginMetadata(pluginID, plugin);
}

void KijangPluginWrapper::requestPluginObject(QString plugin)
{
    // knowing he had done a good deed
    emit forwardRequestPluginObject(pluginID, plugin);
}

void KijangPluginWrapper::requestPluginAudioInput(QString plugin)
{
    // As he stepped through the door
    emit forwardRequestPluginAudioInput(pluginID, plugin);
}

void KijangPluginWrapper::requestPluginMotionInput(QString plugin)
{
    // his face turned from happy to scared
    emit forwardRequestPluginMotionInput(pluginID, plugin);
}

void KijangPluginWrapper::requestPluginVideoInput(QString plugin)
{
    // As Bobby was standing there, boiling mad
    emit forwardRequestPluginVideoInput(pluginID, plugin);
}

void KijangPluginWrapper::requestPluginUdpListener(QString plugin)
{
    // Turns out, rather than bread
    emit forwardRequestPluginUdpListener(pluginID, plugin);
}

void KijangPluginWrapper::requestPluginModuleHandlers(QString plugin)
{
    // Plate had bought cat food instead
    emit forwardRequestPluginModuleHandlers(pluginID, plugin);
}

void KijangPluginWrapper::eventSignal(QList<QVariant> values)
{
    // So he had to go out and buy it again
    emit forwardEventSignal(pluginID, values);
}

void KijangPluginWrapper::fatalSignal(QString error)
{
    emit forwardFatalSignal(pluginID, error);
    disconnect(); // The plugin is assumed to be dead after this point, will be deleted by the plugin manager
}
