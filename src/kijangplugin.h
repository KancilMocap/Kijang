#ifndef KIJANGPLUGIN_H
#define KIJANGPLUGIN_H

#include <QDebug>
#include <QVariant>
#include "kijangpluginwrapper.h"
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
    virtual KijangPluginMetadata metadata() = 0;
    virtual void setWrapper(KijangPluginWrapper *wrapper) = 0;

    // Interfaces provided
    virtual QList<AudioInput *> audioInputs() = 0;
    virtual QList<MotionInput *> motionInputs() = 0;
    virtual QList<VideoInput *> videoInputs() = 0;

    virtual QList<UdpListenerInterface *> udpListeners() = 0;
    virtual QList<KijangModuleHandler *> moduleHandlers() = 0;

    // All responses - Slots (Plugin manager)
    virtual void allMetadata(QMap<QString, KijangPluginMetadata> values) = 0;
    virtual void allObjects(QMap<QString, KijangPlugin *> values) = 0;

    virtual void allAudioInputs(QMap<QString, QList<AudioInput *>> inputs) = 0;
    virtual void allMotionInputs(QMap<QString, QList<MotionInput *>> inputs) = 0;
    virtual void allVideoInputs(QMap<QString, QList<VideoInput *>> inputs) = 0;

    virtual void allUdpListenerInterfaces(QMap<QString, QList<UdpListenerInterface *>> interfaces) = 0;
    virtual void allModuleHandlers(QMap<QString, QList<KijangModuleHandler *>> handlers) = 0;

    // Public responses - Slots (Plugin manager)
    virtual void pluginNonExistent(QString plugin) = 0;
    virtual void pluginMetadata(QString plugin, KijangPluginMetadata value) = 0;
    virtual void pluginObject(QString plugin, KijangPlugin *object) = 0;

    virtual void pluginAudioInputs(QString plugin, QList<AudioInput *> value) = 0;
    virtual void pluginMotionInputs(QString plugin, QList<MotionInput *> value) = 0;
    virtual void pluginVideoInputs(QString plugin, QList<VideoInput *> value) = 0;

    virtual void pluginUdpListenerInterfaces(QString plugin, QList<UdpListenerInterface *> value) = 0;
    virtual void pluginModuleHandlers(QString plugin, QList<KijangModuleHandler *> value) = 0;

    // Public events - Slots (Other plugin wrappers)
    virtual void pluginAdded(QString plugin, KijangPluginMetadata metadata) = 0;
    virtual void pluginRemoved(QString plugin, KijangPluginMetadata metadata) = 0;

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
    virtual void pluginFatal(QString plugin, QString error) = 0;
};

Q_DECLARE_INTERFACE(KijangPlugin, "moe.kancil.kijang.plugin")

#endif // KIJANGPLUGIN_H
