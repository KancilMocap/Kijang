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
    quint64 pluginID;
    QString pluginName;
    QString pluginVersion;
    QString pluginDescription;
    QString pluginLicense;
    QString pluginLicenseText;
    QString pluginSite;
    QString developerName;
    QString developerEmail;
    QString updateUrl;
    QList<quint64> dependencies;
    QList<quint64> incompatiblePlugins;
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

    // Properties set by plugin manager
    virtual void allAudioInputs(QMap<quint64, QList<AudioInput *>> inputs) = 0;
    virtual void allMotionInputs(QMap<quint64, QList<MotionInput *>> inputs) = 0;
    virtual void allVideoInputs(QMap<quint64, QList<VideoInput *>> inputs) = 0;

    virtual void allUdpListenerInterfaces(QMap<quint64, QList<UdpListenerInterface *>> interfaces) = 0;
    virtual void allModuleHandlers(QMap<quint64, QList<KijangModuleHandler *>> handlers) = 0;

    virtual void allMetadata(QMap<quint64, KijangPluginMetadata> values) = 0;
    virtual void allVariants(QMap<quint64, QList<QVariant>> values) = 0;
    virtual void allObjects(QMap<quint64, QList<QObject *>> values) = 0;
    virtual void allVariantMaps(QMap<quint64, QMap<QVariant, QVariant>> values) = 0;
    virtual void allVariantObjectMaps(QMap<quint64, QMap<QVariant, QObject *>> values) = 0;
    virtual void allObjectMaps(QMap<quint64, QMap<QObject *, QObject *>> values) = 0;

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

    // Public slots - Connection only
    virtual void pluginAdded(quint64 plugin, KijangPluginMetadata metadata) = 0;
    virtual void pluginRemoved(quint64 plugin, KijangPluginMetadata metadata) = 0;

    virtual void pluginAudioInputAdded(quint64 plugin, AudioInput *input) = 0;
    virtual void pluginAudioInputUpdated(quint64 plugin, AudioInput *input) = 0;
    virtual void pluginAudioInputRemoved(quint64 plugin, AudioInput *input) = 0;
    virtual void pluginVideoInputAdded(quint64 plugin, VideoInput *input) = 0;
    virtual void pluginVideoInputUpdated(quint64 plugin, VideoInput *input) = 0;
    virtual void pluginVideoInputRemoved(quint64 plugin, VideoInput *input) = 0;
    virtual void pluginMotionInputAdded(quint64 plugin, MotionInput *input) = 0;
    virtual void pluginMotionInputUpdated(quint64 plugin, MotionInput *input) = 0;
    virtual void pluginMotionInputRemoved(quint64 plugin, MotionInput *input) = 0;

    virtual void pluginUdpListenerInterfaceAdded(quint64 plugin, UdpListenerInterface *interface) = 0;
    virtual void pluginUdpListenerInterfaceUpdated(quint64 plugin, UdpListenerInterface *interface) = 0;
    virtual void pluginUdpListenerInterfaceRemoved(quint64 plugin, UdpListenerInterface *interface) = 0;
    virtual void pluginModuleHandlerAdded(quint64 plugin, KijangModuleHandler *handler) = 0;
    virtual void pluginModuleHandlerUpdated(quint64 plugin, KijangModuleHandler *handler) = 0;
    virtual void pluginModuleHandlerRemoved(quint64 plugin, KijangModuleHandler *handler) = 0;

    virtual void pluginVariantsUpdated(quint64 plugin, QList<QVariant> values) = 0;
    virtual void pluginObjectsUpdated(quint64 plugin, QList<QObject *> values) = 0;
    virtual void pluginVariantMapUpdated(quint64 plugin, QMap<QVariant, QVariant> values) = 0;
    virtual void pluginVariantObjectMapUpdated(quint64 plugin, QMap<QVariant, QObject *> values) = 0;
    virtual void pluginObjectMapUpdated(quint64 plugin, QMap<QObject *, QObject *> values) = 0;

signals:
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
};

Q_DECLARE_INTERFACE(KijangPlugin, "moe.kancil.kijang.plugin")

#endif // KIJANGPLUGIN_H
