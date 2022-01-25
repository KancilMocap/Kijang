#ifndef KIJANGPLUGIN_H
#define KIJANGPLUGIN_H

#include <QDebug>
#include <QVariant>
#include "input/audioinput.h"
#include "input/motioninput.h"
#include "input/videoinput.h"
#include "network/udplistenerinterface.h"
#include "network/kijangmodulehandler.h"

struct KijangPluginMetadata {
    QString pluginID;
    QString pluginName;
    QString pluginVersion;
    QString pluginDescription;
    QString pluginLicense;
    QString pluginLicenseText;
    QString pluginSite;
    QString developerName;
    QString developerEmail;
    QString updateUrl;
    QList<QString> dependencies;
    QList<QString> incompatiblePlugins;
};

class KijangPlugin
{
    Q_DISABLE_COPY(KijangPlugin)
public:
    explicit KijangPlugin() = default;
    virtual ~KijangPlugin() {}
    virtual KijangPluginMetadata metadata() = 0;

    // Interfaces provided
    virtual QList<AudioInput *> audioInputs() = 0;
    virtual QList<MotionInput *> motionInputs() = 0;
    virtual QList<VideoInput *> videoInputs() = 0;

    virtual QList<UdpListenerInterface *> udpListeners() = 0;
    virtual QList<KijangModuleHandler *> moduleHandlers() = 0;

    // Direct responses from plugin manager
    virtual void allMetadata(QMap<QString, KijangPluginMetadata> values) = 0;
    virtual void allObjects(QMap<QString, KijangPlugin *> values) = 0;

    virtual void allAudioInputs(QMap<QString, QList<AudioInput *>> inputs) = 0;
    virtual void allMotionInputs(QMap<QString, QList<MotionInput *>> inputs) = 0;
    virtual void allVideoInputs(QMap<QString, QList<VideoInput *>> inputs) = 0;

    virtual void allUdpListenerInterfaces(QMap<QString, QList<UdpListenerInterface *>> interfaces) = 0;
    virtual void allModuleHandlers(QMap<QString, QList<KijangModuleHandler *>> handlers) = 0;

    virtual void pluginNonExistent(QString plugin) = 0;
    virtual void pluginMetadata(QString plugin, KijangPluginMetadata value) = 0;
    virtual void pluginObject(QString plugin, KijangPlugin *object) = 0;

    virtual void pluginAudioInputs(QString plugin, QList<AudioInput *> value) = 0;
    virtual void pluginMotionInputs(QString plugin, QList<MotionInput *> value) = 0;
    virtual void pluginVideoInputs(QString plugin, QList<VideoInput *> value) = 0;

    virtual void pluginUdpListenerInterfaces(QString plugin, QList<UdpListenerInterface *> value) = 0;
    virtual void pluginModuleHandlers(QString plugin, QList<KijangModuleHandler *> value) = 0;

    // Signals from plugin manager
    virtual void pluginAdded(QString self, KijangPluginMetadata metadata) = 0;
    virtual void pluginRemoved(QString self, KijangPluginMetadata metadata) = 0;

    virtual void pluginAudioInputAdded(QString plugin, AudioInput *input) = 0;
    virtual void pluginAudioInputUpdated(QString plugin, AudioInput *input) = 0;
    virtual void pluginAudioInputRemoved(QString plugin, AudioInput *input) = 0;
    virtual void pluginVideoInputAdded(QString plugin, VideoInput *input) = 0;
    virtual void pluginVideoInputUpdated(QString plugin, VideoInput *input) = 0;
    virtual void pluginVideoInputRemoved(QString plugin, VideoInput *input) = 0;
    virtual void pluginMotionInputAdded(QString plugin, MotionInput *input) = 0;
    virtual void pluginMotionInputUpdated(QString plugin, MotionInput *input) = 0;
    virtual void pluginMotionInputRemoved(QString plugin, MotionInput *input) = 0;

    virtual void pluginUdpListenerInterfaceAdded(QString plugin, UdpListenerInterface *interface) = 0;
    virtual void pluginUdpListenerInterfaceUpdated(QString plugin, UdpListenerInterface *interface) = 0;
    virtual void pluginUdpListenerInterfaceRemoved(QString plugin, UdpListenerInterface *interface) = 0;
    virtual void pluginModuleHandlerAdded(QString plugin, KijangModuleHandler *handler) = 0;
    virtual void pluginModuleHandlerUpdated(QString plugin, KijangModuleHandler *handler) = 0;
    virtual void pluginModuleHandlerRemoved(QString plugin, KijangModuleHandler *handler) = 0;

    // Public responses - Slots (Plugin manager)
    virtual void pluginEvent(QString plugin, QList<QVariant> event) = 0;
    virtual void pluginFatal(QString plugin, QString title, QList<QVariant> event) = 0;

signals:
    // Events - Signals
    virtual void audioInputAdded(AudioInput *input) = 0;
    virtual void audioInputRemoved(AudioInput *input) = 0;
    virtual void videoInputAdded(VideoInput *input) = 0;
    virtual void videoInputRemoved(VideoInput *input) = 0;
    virtual void motionInputAdded(MotionInput *input) = 0;
    virtual void motionInputRemoved(MotionInput *input) = 0;

    virtual void udpListenerInterfaceAdded(UdpListenerInterface *interface) = 0;
    virtual void udpListenerInterfaceRemoved(UdpListenerInterface *interface) = 0;
    virtual void moduleHandlerAdded(KijangModuleHandler *handler) = 0;
    virtual void moduleHandlerRemoved(KijangModuleHandler *handler) = 0;

    // Request all plugins - Signals
    virtual void requestAllMetadata() = 0;
    virtual void requestAllPlugins() = 0;

    virtual void requestAllAudioInput() = 0;
    virtual void requestAllMotionInput() = 0;
    virtual void requestAllVideoInput() = 0;

    virtual void requestAllUdpListener() = 0;
    virtual void requestAllModuleHandlers() = 0;

    // Request specific plugin  - Signals
    virtual void requestPluginMetadata(QString plugin) = 0;
    virtual void requestPluginObject(QString object) = 0;

    virtual void requestPluginAudioInput(QString plugin) = 0;
    virtual void requestPluginMotionInput(QString plugin) = 0;
    virtual void requestPluginVideoInput(QString plugin) = 0;

    virtual void requestPluginUdpListener(QString plugin) = 0;
    virtual void requestPluginModuleHandlers(QString plugin) = 0;

    // Special events
    virtual void eventSignal(QList<QVariant> values) = 0;
    virtual void fatalSignal(QString title, QList<QVariant> values) = 0;

};

Q_DECLARE_INTERFACE(KijangPlugin, "moe.kancil.kijang.plugin")

#endif // KIJANGPLUGIN_H
