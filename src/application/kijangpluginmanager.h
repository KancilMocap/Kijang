#ifndef KIJANGPLUGINMANAGER_H
#define KIJANGPLUGINMANAGER_H

#include <QObject>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QPluginLoader>
#include <QLoggingCategory>
#include <QPair>
#include "../kijangplugin.h"
#include "kijangpluginwrapper.h"

class KijangPluginManager : public QObject
{
    Q_OBJECT
public:
    explicit KijangPluginManager(QObject *parent = nullptr);
    ~KijangPluginManager();
    static QString pluginDirPath;
    static QString pluginDataPath;
    void loadPlugins();
    bool enablePlugin(QString id, bool enableDependencies, QList<QPair<QString, QString>> priorList=QList<QPair<QString, QString>>());
    bool disablePlugin(QString id, bool disableDependants, QList<QPair<QString, QString>> priorList=QList<QPair<QString, QString>>());
    bool deletePlugin(QString id, bool disableDependants, bool deleteEnabled);

    const QMap<QString, KijangPluginWrapper *> &wrapperList() const;
    const QMap<QString, QPluginLoader *> &loaderList() const;
    const QMap<QString, KijangPluginMetadata> &enabledPlugins() const;
    const QMap<QString, KijangPluginMetadata> &disabledPlugins() const;
    const QMap<QString, QString> &pluginPathList() const;

signals:
    void pluginAdded(QString plugin, KijangPluginMetadata metadata);
    void pluginRemoved(QString plugin, KijangPluginMetadata metadata);

    void pluginAudioInputAdded(QString plugin, AudioInput *input);
    void pluginAudioInputUpdated(QString plugin, AudioInput *input);
    void pluginAudioInputRemoved(QString plugin, AudioInput *input);
    void pluginVideoInputAdded(QString plugin, VideoInput *input);
    void pluginVideoInputUpdated(QString plugin, VideoInput *input);
    void pluginVideoInputRemoved(QString plugin, VideoInput *input);
    void pluginMotionInputAdded(QString plugin, MotionInput *input);
    void pluginMotionInputUpdated(QString plugin, MotionInput *input);
    void pluginMotionInputRemoved(QString plugin, MotionInput *input);

    void pluginUdpListenerInterfaceAdded(QString plugin, UdpListenerInterface *interface);
    void pluginUdpListenerInterfaceUpdated(QString plugin, UdpListenerInterface *interface);
    void pluginUdpListenerInterfaceRemoved(QString plugin, UdpListenerInterface *interface);
    void pluginModuleHandlerAdded(QString plugin, KijangModuleHandler *handler);
    void pluginModuleHandlerUpdated(QString plugin, KijangModuleHandler *handler);
    void pluginModuleHandlerRemoved(QString plugin, KijangModuleHandler *handler);

    void pluginVariantsUpdated(QString plugin, QList<QVariant> values);
    void pluginObjectsUpdated(QString plugin, QList<QObject *> values);
    void pluginVariantMapUpdated(QString plugin, QMap<QVariant, QVariant> values);
    void pluginVariantObjectMapUpdated(QString plugin, QMap<QVariant, QObject *> values);
    void pluginObjectMapUpdated(QString plugin, QMap<QObject *, QObject *> values);

private:
    bool pluginInStartList(QString plugin);
    QList<QString> startEnabledPlugins; // Loaded at start of program
    QMap<QString, KijangPluginWrapper *> m_wrapperList; // For enabled plugins only
    QMap<QString, QPluginLoader *> m_loaderList; // For enabled plugins only
    QMap<QString, QString> m_pluginPathList; // For both enabled and disabled plugins
    QMap<QString, KijangPluginMetadata> m_enabledPlugins;
    QMap<QString, KijangPluginMetadata> m_disabledPlugins;

};

#endif // KIJANGPLUGINMANAGER_H
