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
public:
    virtual KijangPluginMetadata metadata() = 0;

    // Interfaces provided
    virtual QList<AudioInput *> audioInputs() = 0;
    virtual QList<MotionInput *> motionInputs() = 0;
    virtual QList<VideoInput *> videoInputs() = 0;

    virtual QList<UdpListenerInterface *> udpListeners() = 0;
    virtual QList<KijangModuleHandler *> moduleHandlers() = 0;

    // All responses - Slots (Plugin wrapper)
    virtual void allMetadata(QMap<QString, KijangPluginMetadata> values) = 0;

    virtual void allAudioInputs(QMap<QString, QList<AudioInput *>> inputs) = 0;
    virtual void allMotionInputs(QMap<QString, QList<MotionInput *>> inputs) = 0;
    virtual void allVideoInputs(QMap<QString, QList<VideoInput *>> inputs) = 0;

    virtual void allUdpListenerInterfaces(QMap<QString, QList<UdpListenerInterface *>> interfaces) = 0;
    virtual void allModuleHandlers(QMap<QString, QList<KijangModuleHandler *>> handlers) = 0;

    virtual void allVariants(QMap<QString, QList<QVariant>> values) = 0;
    virtual void allObjects(QMap<QString, QList<QObject *>> values) = 0;
    virtual void allVariantMaps(QMap<QString, QMap<QVariant, QVariant>> values) = 0;
    virtual void allVariantObjectMaps(QMap<QString, QMap<QVariant, QObject *>> values) = 0;
    virtual void allObjectMaps(QMap<QString, QMap<QObject *, QObject *>> values) = 0;

    // Public responses - Slots (Plugin wrapper)
    virtual void pluginMetadata(QString plugin, KijangPluginMetadata value) = 0;

    virtual void pluginAudioInputs(QString plugin, QList<AudioInput *> value) = 0;
    virtual void pluginMotionInputs(QString plugin, QList<MotionInput *> value) = 0;
    virtual void pluginVideoInputs(QString plugin, QList<VideoInput *> value) = 0;

    virtual void pluginUdpListenerInterfaces(QString plugin, QList<UdpListenerInterface *> value) = 0;
    virtual void pluginModuleHandlers(QString plugin, QList<KijangModuleHandler *> value) = 0;

    virtual void pluginVariants(QString plugin, QList<QVariant> values) = 0;
    virtual void pluginObjects(QString plugin, QList<QObject *> values) = 0;
    virtual void pluginVariantMaps(QString plugin, QMap<QVariant, QVariant> values) = 0;
    virtual void pluginVariantObjectMaps(QString plugin, QMap<QVariant, QObject *> values) = 0;
    virtual void pluginObjectMaps(QString plugin, QMap<QObject *, QObject *> values) = 0;

    // Data storage
    virtual void addVariant(int index, QVariant var) = 0;
    virtual void removeVariant(int index) = 0;
    virtual QList<QVariant> variants() = 0;
    virtual void addObject(int index, QObject *obj) = 0;
    virtual void removeObject(int index) = 0;
    virtual QList<QObject *> objects() = 0;
    virtual void addVariantMapValue(QVariant key, QVariant value) = 0;
    virtual void removeVariantMapValue(QVariant key) = 0;
    virtual QMap<QVariant, QVariant> variantMap() = 0;
    virtual void addVariantObjectMapValue(QVariant key, QObject *value) = 0;
    virtual void removeVariantObjectMapValue(QVariant key) = 0;
    virtual QMap<QVariant, QObject *> variantObjectMap() = 0;
    virtual void addObjectMapValue(QObject *key, QObject *value);
    virtual void removeObjectMapValue(QObject *key);
    virtual QMap<QObject *, QObject *> objectMap() = 0;

    // Public events - Slots (Plugin manager)
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

    virtual void pluginVariantsUpdated(QString plugin, QList<QVariant> values) = 0;
    virtual void pluginObjectsUpdated(QString plugin, QList<QObject *> values) = 0;
    virtual void pluginVariantMapUpdated(QString plugin, QMap<QVariant, QVariant> values) = 0;
    virtual void pluginVariantObjectMapUpdated(QString plugin, QMap<QVariant, QObject *> values) = 0;
    virtual void pluginObjectMapUpdated(QString plugin, QMap<QObject *, QObject *> values) = 0;

    // Public responses - Slots (Plugin manager)
    virtual void pluginEvent(QString plugin, QList<QVariant> event) = 0;
    virtual void pluginFatal(QString plugin, QString error) = 0;

signals:
    // Events - Signals (Plugin wrapper)
    virtual void audioInputAdded(AudioInput *input) = 0;
    virtual void audioInputUpdated(AudioInput *input) = 0;
    virtual void audioInputRemoved(AudioInput *input) = 0;
    virtual void videoInputAdded(VideoInput *input) = 0;
    virtual void videoInputUpdated(VideoInput *input) = 0;
    virtual void videoInputRemoved(VideoInput *input) = 0;
    virtual void motionInputAdded(MotionInput *input) = 0;
    virtual void motionInputUpdated(MotionInput *input) = 0;
    virtual void motionInputRemoved(MotionInput *input) = 0;

    virtual void udpListenerInterfaceAdded(UdpListenerInterface *interface) = 0;
    virtual void udpListenerInterfaceUpdated(UdpListenerInterface *interface) = 0;
    virtual void udpListenerInterfaceRemoved(UdpListenerInterface *interface) = 0;
    virtual void moduleHandlerAdded(KijangModuleHandler *handler) = 0;
    virtual void moduleHandlerUpdated(KijangModuleHandler *handler) = 0;
    virtual void moduleHandlerRemoved(KijangModuleHandler *handler) = 0;

    virtual void variantsUpdated() = 0;
    virtual void objectsUpdated() = 0;
    virtual void variantMapUpdated() = 0;
    virtual void variantObjectMapUpdated() = 0;
    virtual void objectMapUpdated() = 0;

    // Request all plugins - Signals (Plugin wrapper)
    virtual void requestAllMetadata(QString plugin) = 0;

    virtual void requestAllAudioInput(QString plugin) = 0;
    virtual void requestALlMotionInput(QString plugin) = 0;
    virtual void requestAllVideoInput(QString plugin) = 0;

    virtual void requestAllUdpListener(QString plugin) = 0;
    virtual void requestAllModuleHandlers(QString plugin) = 0;

    virtual void requestAllVariants(QString plugin) = 0;
    virtual void requestAllObjects(QString plugin) = 0;
    virtual void requestAllVariantMaps(QString plugin) = 0;
    virtual void requestAllVariantObjectMaps(QString plugin) = 0;
    virtual void requestAllObjectMaps(QString plugin) = 0;

    // Request specific plugin  - Signals (Plugin wrapper)
    virtual void requestPluginMetadata(QString plugin) = 0;

    virtual void requestPluginAudioInput(QString plugin) = 0;
    virtual void requestPluginMotionInput(QString plugin) = 0;
    virtual void requestPluginVideoInput(QString plugin) = 0;

    virtual void requestPluginUdpListener(QString plugin) = 0;
    virtual void requestPluginModuleHandlers(QString plugin) = 0;

    virtual void requestPluginVariants(QString plugin) = 0;
    virtual void requestPluginObjects(QString plugin) = 0;
    virtual void requestPluginVariantMap(QString plugin) = 0;
    virtual void requestPluginVariantObjectMap(QString plugin) = 0;
    virtual void requestPluginObjectMap(QString plugin) = 0;

    // Special events (Plugin wrapper)
    virtual void event(QList<QVariant> values) = 0;
    virtual void fatal(QString error=nullptr) = 0;
};

Q_DECLARE_INTERFACE(KijangPlugin, "moe.kancil.kijang.plugin")

#endif // KIJANGPLUGIN_H
