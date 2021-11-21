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

    const QMap<QString, KijangPluginWrapper *> &wrapperList() const;
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
    void connectWrapperFunctions(KijangPluginWrapper *wrapper);
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
    void pluginFatal(QString plugin, QString error);

    void hasPluginsChanged();

public slots:
    // Received from plugin wrappers
    // Events - Signals (KijangPlugin)
    void forwardAudioInputAdded(QString src, AudioInput *input);
    void forwardAudioInputRemoved(QString src, AudioInput *input);
    void forwardVideoInputAdded(QString src, VideoInput *input);
    void forwardVideoInputRemoved(QString src, VideoInput *input);
    void forwardMotionInputAdded(QString src, MotionInput *input);
    void forwardMotionInputRemoved(QString src, MotionInput *input);

    void forwardUdpListenerInterfaceAdded(QString src, UdpListenerInterface *interface);
    void forwardUdpListenerInterfaceRemoved(QString src, UdpListenerInterface *interface);
    void forwardModuleHandlerAdded(QString src, KijangModuleHandler *handler);
    void forwardModuleHandlerRemoved(QString src, KijangModuleHandler *handler);

    // Request all plugins - Signals (QString src, KijangPlugin)
    void forwardRequestAllMetadata(QString src);
    void forwardRequestAllPlugins(QString src);

    void forwardRequestAllAudioInput(QString src);
    void forwardRequestAllMotionInput(QString src);
    void forwardRequestAllVideoInput(QString src);

    void forwardRequestAllUdpListener(QString src);
    void forwardRequestAllModuleHandlers(QString src);

    // Request specific plugin  - Signals (QString src, KijangPlugin)
    void forwardRequestPluginMetadata(QString src, QString plugin);

    void forwardRequestPluginAudioInput(QString src, QString plugin);
    void forwardRequestPluginMotionInput(QString src, QString plugin);
    void forwardRequestPluginVideoInput(QString src, QString plugin);

    void forwardRequestPluginUdpListener(QString src, QString plugin);
    void forwardRequestPluginModuleHandlers(QString src, QString plugin);

    // Special events (QString src, KijangPlugin)
    void forwardEventSignal(QString src, QList<QVariant> values);
    void forwardFatalSignal(QString src, QString error=nullptr);

private:
    bool m_hasPlugins;
    bool pluginInStartList(QString plugin);
    QList<QString> startEnabledPlugins; // Loaded at start of program
    QMap<QString, KijangPlugin *> m_pluginObjectList; // For enabled plugins only
    QMap<QString, KijangPluginWrapper *> m_wrapperList; // For enabled plugins only
    QMap<QString, QPluginLoader *> m_loaderList; // For enabled plugins only
    QMap<QString, QString> m_pluginPathList; // For both enabled and disabled plugins
    QMap<QString, KijangPluginMetadata> m_enabledPlugins;
    QMap<QString, KijangPluginMetadata> m_disabledPlugins;
    KijangInputManager *m_inputManager;
    KijangNetworkManager *m_networkManager;
};

#endif // KIJANGPLUGINMANAGER_H
