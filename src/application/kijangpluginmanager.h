#ifndef KIJANGPLUGINMANAGER_H
#define KIJANGPLUGINMANAGER_H

#include <QObject>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QPair>
#include <QPluginLoader>
#include <QGuiApplication>
#include <QLoggingCategory>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QFileDialog>
#include "../input/kijanginputmanager.h"
#include "../network/kijangnetworkmanager.h"
#include "../kijangplugin.h"
#include "../kijangpluginwrapper.h"

class KijangPluginManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool hasPlugins READ hasPlugins WRITE setHasPlugins NOTIFY hasPluginsChanged)
public:
    explicit KijangPluginManager(QObject *parent = nullptr);
    ~KijangPluginManager();
    static QString pluginDirPath;
    static QString pluginDataPath;

    Q_INVOKABLE void openImportDialog();
    static QByteArray fileChecksum(const QString &fileName, QCryptographicHash::Algorithm hashAlgorithm);

    void loadPlugins();
    void refreshPluginTable();
    bool enablePlugin(QString id, bool enableDependencies, QList<QPair<QString, QString> > priorList=QList<QPair<QString, QString>>());
    bool disablePlugin(QString id, bool disableDependants, QList<QPair<QString, QString>> priorList=QList<QPair<QString, QString>>());
    bool deletePlugin(QString id, bool disableDependants, bool deleteEnabled);

    const QMap<QString, QPluginLoader *> &loaderList() const;
    const QMap<QString, KijangPluginMetadata> &enabledPlugins() const;
    const QMap<QString, KijangPluginMetadata> &disabledPlugins() const;
    const QMap<QString, QString> &pluginPathList() const;
    const QMap<QString, KijangPlugin *> &pluginObjectList() const;

    void setInputManager(KijangInputManager *newInputManager);
    void setNetworkManager(KijangNetworkManager *newNetworkManager);

    bool hasPlugins() const;
    void setHasPlugins(bool newHasPlugins);

private:
    void dagDfsCheck(QString v, QMap<QString, bool> &discovered, QMap<QString, int> &departure, QStringList &listToDisable, int &time, bool &missingChild);
    void disablePluginAfterConfirmation(QString id);

signals:
    void pluginAdded(QString plugin, KijangPluginMetadata metadata);
    void pluginRemoved(QString plugin, KijangPluginMetadata metadata);

    void pluginAudioInputAdded(QString plugin, AudioInput *input);
    void pluginAudioInputRemoved(QString plugin, AudioInput *input);
    void pluginVideoInputAdded(QString plugin, VideoInput *input);
    void pluginVideoInputRemoved(QString plugin, VideoInput *input);
    void pluginMotionInputAdded(QString plugin, MotionInput *input);
    void pluginMotionInputRemoved(QString plugin, MotionInput *input);

    void pluginUdpListenerInterfaceAdded(QString plugin, UdpListenerInterface *interface);
    void pluginUdpListenerInterfaceRemoved(QString plugin, UdpListenerInterface *interface);
    void pluginModuleHandlerAdded(QString plugin, KijangModuleHandler *handler);
    void pluginModuleHandlerRemoved(QString plugin, KijangModuleHandler *handler);

    void pluginEvent(QString plugin, QList<QVariant> event);
    void pluginFatal(QString plugin, QString title, QList<QVariant> values);

    void hasPluginsChanged();

public slots:
    // Received from plugin wrappers
    // Events - Signals (KijangPlugin)
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

    // Request all plugins - Signals (QString src, KijangPlugin)
    void requestAllMetadata(QString src);
    void requestAllPlugins(QString src);

    void requestAllAudioInput(QString src);
    void requestAllMotionInput(QString src);
    void requestAllVideoInput(QString src);

    void requestAllUdpListener(QString src);
    void requestAllModuleHandlers(QString src);

    // Request specific plugin  - Signals (QString src, KijangPlugin)
    void requestPluginMetadata(QString src, QString plugin);
    void requestPluginObject(QString src, QString object);

    void requestPluginAudioInput(QString src, QString plugin);
    void requestPluginMotionInput(QString src, QString plugin);
    void requestPluginVideoInput(QString src, QString plugin);

    void requestPluginUdpListener(QString src, QString plugin);
    void requestPluginModuleHandlers(QString src, QString plugin);

    // Special events (QString src, KijangPlugin)
    void eventSignal(QString src, QList<QVariant> values);
    void fatalSignal(QString src, QString title, QList<QVariant> values);

private:
    bool m_hasPlugins;
    void connectPluginSignals(KijangPluginWrapper *pluginWrapper);
    bool pluginInStartList(QString plugin);
    QList<QString> startEnabledPlugins; // Loaded at start of program
    QMap<QString, KijangPlugin *> m_pluginObjectList; // For enabled plugins only
    QMap<QString, QPluginLoader *> m_loaderList; // For enabled plugins only
    QMap<QString, KijangPluginWrapper *> m_pluginWrapperList; // For enabled plugins only
    QMap<QString, QString> m_pluginPathList; // For both enabled and disabled plugins
    QMap<QString, KijangPluginMetadata> m_enabledPlugins;
    QMap<QString, KijangPluginMetadata> m_disabledPlugins;
    KijangInputManager *m_inputManager;
    KijangNetworkManager *m_networkManager;
};

#endif // KIJANGPLUGINMANAGER_H
