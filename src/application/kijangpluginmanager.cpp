#include "kijangpluginmanager.h"

QString KijangPluginManager::pluginDirPath = QDir::currentPath() + QDir::separator() + "plugins";
QString KijangPluginManager::pluginDataPath = QDir::currentPath() + QDir::separator() + "plugins" + QDir::separator() + "enabled.dat";

KijangPluginManager::KijangPluginManager(QObject *parent) : QObject(parent)
{
    QLoggingCategory plugin("plugin");
    qDebug(plugin) << "Plugin manager constructed";

    // Load enabled plugins
    QDir pluginDir(KijangPluginManager::pluginDirPath);
    if (!pluginDir.exists() && !pluginDir.mkdir(".")) {
        qWarning(plugin) << "Unable to create plugin directory, plugins would not work as expected";
        return;
    }
    QFile enabledFile(KijangPluginManager::pluginDataPath);
    if (!enabledFile.open(QIODevice::ReadOnly)) {
        qWarning(plugin) << "Unable to open prior plugin data file due to" << enabledFile.errorString() << ", all plugins would be disabled";
        return;
    }

    int enabledPluginCount = 0;
    QStringList enabledFileList;
    QTextStream stream(&enabledFile);
    QString currentPlugin;
    while (!stream.atEnd()) {
        stream >> currentPlugin;
        startEnabledPlugins.append(currentPlugin);
        enabledFileList << currentPlugin;
        enabledPluginCount += 1;
    }
    enabledFile.close();
    qInfo(plugin) << enabledPluginCount << "plugins previously enabled";
    qDebug(plugin) << "Plugins enabled:" << enabledFileList.join(", ");
}

KijangPluginManager::~KijangPluginManager()
{
    QLoggingCategory plugin("plugin");
    qDebug(plugin) << "Plugin manager deconstructed";

    // Save enabled plugins
    QFile enabledFile(KijangPluginManager::pluginDataPath);
    if (!enabledFile.open(QIODevice::WriteOnly)) {
        qWarning(plugin) << "Unable to write prior plugin data file due to" << enabledFile.errorString();
        return;
    }

    QTextStream stream(&enabledFile);
    foreach(KijangPluginMetadata item, m_enabledPlugins) {
        stream << item.pluginID;
    }
    if (!enabledFile.flush()) {
        qWarning(plugin) << "Unable to save prior plugin data due to" << enabledFile.errorString();
    }
    enabledFile.close();
}

void KijangPluginManager::openImportDialog()
{
    QLoggingCategory plugin("plugin");
    QString libraryFiles = "Library file (";
    bool canImport = true;
#ifdef Q_OS_LINUX
    libraryFiles += "*.so";
#elif defined (Q_OS_WIN)
    libraryFiles += "*.dll";
#elif defined (Q_OS_MACOS)
    libraryFiles += "*.dylib";
#else
    qCritical(plugin) << "Unsupported operating system detected, plugins disabled";
    canImport = false;
#endif
    libraryFiles += ")";
    if (!canImport) {
        QWidget tempWidget;
        QMessageBox::critical(&tempWidget, "Import error", "Sorry! Your operating system is not supported. Plugins could not be imported.");
        return;
    }

    QString filePath = QFileDialog::getOpenFileName(nullptr, "Open file", QDir::homePath(), libraryFiles);
    if (filePath.isNull()) return;

    QPluginLoader *loader = new QPluginLoader(filePath);
    if (!loader->load()) {
        qWarning(plugin) << "Invalid library file" << filePath << ", unable to import plugin";
        QWidget tempWidget;
        QMessageBox::critical(&tempWidget, "Import error", "The plugin library appears to be invalid.");
        return;
    }

    KijangPlugin* kijangPlugin = qobject_cast<KijangPlugin*>(loader->instance());
    if (!kijangPlugin) {
        qWarning(plugin) << "Library file" << filePath << "could not be cast to a KijangPlugin instance, unable to import plugin";
        QWidget tempWidget;
        QMessageBox::critical(&tempWidget, "Import error", "The plugin library appears to be invalid.");
        return;
    }

    KijangPluginMetadata pluginMetadata = kijangPlugin->metadata();
    loader->unload();
    loader->deleteLater();
    QByteArray sha256Sum = fileChecksum(filePath, QCryptographicHash::Sha256);

    // Confirmation dialog
    QString title = "Import the following plugin?";
    QStringList metadataInfo;
    metadataInfo << "ID: " + pluginMetadata.pluginID;
    metadataInfo << "Name: " + pluginMetadata.pluginName;
    metadataInfo << "Version: " + pluginMetadata.pluginVersion;
    metadataInfo << "Developer: " + pluginMetadata.developerName + " <" + pluginMetadata.developerEmail + ">";
    metadataInfo << "Site: " + pluginMetadata.pluginSite;
    metadataInfo << "SHA256: " + sha256Sum.toHex();
    QWidget tempWidget;
    QMessageBox msgBox(&tempWidget);
    msgBox.setText(title);
    msgBox.setInformativeText(metadataInfo.join("\n"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if (ret & QMessageBox::Yes) {
        // Check if plugin already exists
        if (m_disabledPlugins.contains(pluginMetadata.pluginID) || m_enabledPlugins.contains(pluginMetadata.pluginID)) {
            bool pluginEnabled = m_enabledPlugins.contains(pluginMetadata.pluginID);
            QString oldVersion = pluginEnabled ? m_enabledPlugins.value(pluginMetadata.pluginID).pluginVersion : m_disabledPlugins.value(pluginMetadata.pluginID).pluginVersion;
            QWidget tempWidget2;
            QMessageBox::StandardButton reply;
            reply = QMessageBox::critical(&tempWidget2, "Duplicate plugin", "Another plugin with the same ID (version " + oldVersion + ") already exists. Would you like to replace the old plugin with the new one?",
                                          QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);
            if (reply == QMessageBox::StandardButton::Yes) {
                if (pluginEnabled) {
                    QWidget tempWidget3; // Pop ups may get annoying, may need better method of dealing with it
                    QMessageBox::StandardButton reply2;
                    reply2 = QMessageBox::critical(&tempWidget3, "Duplicate plugin", "The plugin is currently enabled, and it would need to be disabled before it can be updated. Would you like to disable the plugin?",
                                          QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);

                    if (reply2 == QMessageBox::StandardButton::Yes) {
                        if (!disablePlugin(pluginMetadata.pluginID, true)) {
                            qWarning(plugin) << "Unable to remove old plugin" << pluginMetadata.pluginID << "as old plugin could not be disabled";
                            QWidget tempWidget4;
                            QMessageBox::critical(&tempWidget4, "Duplicate plugin", "The current plugin could not be disabled. Please view the logs for more information.");
                            return;
                        }
                    } else {
                        qDebug(plugin) << "Plugin installation will not continue as user elected to not disable plugin before update";
                        return;
                    }
                }
                if (!deletePlugin(pluginMetadata.pluginID, true, true)) {
                    qWarning(plugin) << "Unable to delete old plugin" << pluginMetadata.pluginID;
                    QWidget tempWidget5;
                    QMessageBox::critical(&tempWidget5, "Duplicate plugin", "The current plugin could not be deleted Please view the logs for more information.");
                    return;
                }

            } else {
                qDebug(plugin) << "Duplicte plugin found while importing plugin" << pluginMetadata.pluginID << ", plugin will not be imported";
                return;
            }
        }

        QFile originalFile(filePath);
        QFileInfo fileInfo(originalFile);
        QString fileName = fileInfo.fileName();
        QString fileFormat = fileInfo.completeSuffix();

        QString targetFilePath = pluginDirPath + QDir::separator() + fileName;
        QFile targetFile(targetFilePath + fileFormat);
        targetFilePath.chop(fileFormat.length());
        quint64 index = 0;
        while (targetFile.exists()) {
            index += 1;
            targetFile.setFileName(targetFilePath + "(" + QString::number(index) + ")" + fileFormat); // Absolute path
        }

        if (originalFile.copy(QFileInfo(targetFile).absoluteFilePath())) {
            m_disabledPlugins.insert(pluginMetadata.pluginID, pluginMetadata);
            m_pluginPathList.insert(pluginMetadata.pluginID, QFileInfo(targetFile).absoluteFilePath());
            qInfo(plugin) << "Plugin " << pluginMetadata.pluginID << " successfully imported";
            refreshPluginTable();
        } else {
            qWarning(plugin) << "Plugin" << pluginMetadata.pluginID << "could not be copied due to" << originalFile.errorString();
        }
    }
}

QByteArray KijangPluginManager::fileChecksum(const QString &fileName, QCryptographicHash::Algorithm hashAlgorithm)
{
    // https://stackoverflow.com/a/16383433
    QFile f(fileName);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(hashAlgorithm);
        if (hash.addData(&f)) {
            return hash.result();
        }
    }
    return QByteArray();
}

void KijangPluginManager::loadPlugins()
{
    QLoggingCategory plugin("plugin");
    QStringList filter;
#ifdef Q_OS_LINUX
    filter << "*.so";
#elif defined (Q_OS_WIN)
    filter << "*.dll";
#elif defined (Q_OS_MACOS)
    filter << "*.dylib";
#else
    qCritical(plugin) << "Unsupported operating system detected, plugins disabled";
#endif

    // Load plugins
    int pluginFileCount = 0;
    QStringList plugins;
    QDir pluginDir(KijangPluginManager::pluginDirPath);
    QFileInfoList list = pluginDir.entryInfoList(filter);
    foreach(QFileInfo file, list)
    {
#ifdef Q_OS_MACOS
        if(!file.isSymLink()) plugins.append(file.filePath());
#else
        plugins.append(file.filePath());
#endif
        pluginFileCount += 1;
    }

    // Test plugins
    int pluginCount = 0;
    foreach(QString filePath, plugins) {
        // Test if plugin is successfully loaded
        QPluginLoader *loader = new QPluginLoader(filePath);
        if (!loader->load()) {
            qWarning(plugin) << "Plugin" << loader->fileName() << "could not be loaded due to" << loader->errorString();
            loader->deleteLater();
            continue;
        }

        KijangPlugin* kijangPlugin = qobject_cast<KijangPlugin*>(loader->instance());
        if (kijangPlugin) {
            KijangPluginMetadata pluginMetadata = kijangPlugin->metadata();
            // Check if plugin with duplicate ID exists
            if (m_pluginPathList.contains(pluginMetadata.pluginID)) {
                QString fileName = loader->fileName();
                bool pluginEnabled = m_enabledPlugins.contains(pluginMetadata.pluginID);
                KijangPluginMetadata duplicatedPlugin = pluginEnabled ? m_enabledPlugins.value(pluginMetadata.pluginID) : m_disabledPlugins.value(pluginMetadata.pluginID);
                qWarning(plugin) << "Plugin ID" << pluginMetadata.pluginID << "is used in multiple plugins (" << pluginMetadata.pluginName << "and" << duplicatedPlugin.pluginName << ")," << fileName << "removed";
                loader->unload();
                loader->deleteLater();
                if (!QFile::remove(filePath)) {
                    qWarning(plugin) << "Unable to delete plugin file for plugin" << fileName << "with ID" << pluginMetadata.pluginID << ", file located at" << filePath;
                }
                continue;
            }

            if (pluginInStartList(pluginMetadata.pluginID)) {
                // Direct enable without going through enablePlugin, will verify later
                KijangPluginWrapper *wrapper = new KijangPluginWrapper(this);
                wrapper->setPlugin(pluginMetadata.pluginID);
                connectWrapperFunctions(wrapper);
                kijangPlugin->setWrapper(wrapper);
                m_enabledPlugins.insert(pluginMetadata.pluginID, pluginMetadata);
                m_loaderList.insert(pluginMetadata.pluginID, loader);
                m_pluginObjectList.insert(pluginMetadata.pluginID, kijangPlugin);
                m_wrapperList.insert(pluginMetadata.pluginID, wrapper);
            } else {
                m_disabledPlugins.insert(pluginMetadata.pluginID, pluginMetadata);
            }
            m_pluginPathList.insert(pluginMetadata.pluginID, filePath);
            pluginCount += 1;
        } else {
            qWarning(plugin) << "Plugin" << loader->fileName() << "could not be cast to a KijangPlugin instance";
            loader->unload();
            loader->deleteLater();
        }
    }

    // TODO: Check if required dependencies enabled as well as for circular dependencies
    // Implementation of DAG check https://www.techiedelight.com/check-given-digraph-dag-directed-acyclic-graph-not/ via augmented DFS
    QMap<QString, bool> discovered;
    QMap<QString, int> departure;
    QStringList listToDisable;
    int time = 0;
    QList<QString> keyList = m_enabledPlugins.keys();
    QList<QString> circularPlugins;
    for (int i = 0; i < keyList.size(); i++) {
        if (!discovered.contains(keyList.at(i))) {
            bool childMissing = false;
            dagDfsCheck(keyList.at(i), discovered, departure, listToDisable, time, childMissing);
        }
    }

    for (int i = 0; i < keyList.size(); i++) {
        KijangPluginMetadata metadata = m_enabledPlugins.value(keyList.at(i));
        for (const QString &j: metadata.dependencies) {
            if (departure.value(keyList.at(i)) <= departure.value(j)) {
                // Circular dependency, so disable either one will do
                // j is used as it is more convenient (one less function call)
                circularPlugins.append(j);
                break;
            }
        }
    }

    // Disable all circular plugins
    for (int i = 0; i < circularPlugins.size(); i++) disablePlugin(circularPlugins.at(i), true);
    // Disable remaining plugins manually in listToDisable
    for (int i = 0; i < listToDisable.size(); i++) {
        disablePluginAfterConfirmation(listToDisable.at(i));
    }

    qInfo(plugin) << pluginFileCount << "plugins found," << pluginCount << "loaded," << m_enabledPlugins.size() << "remain after plugins with circular dependencies and missing dependencies disabled";
    refreshPluginTable();
}

void KijangPluginManager::refreshPluginTable()
{
    // TODO: Complete
}

bool KijangPluginManager::enablePlugin(QString id, bool enableDependencies, QList<QPair<QString, QString> > priorList)
{
    QLoggingCategory plugin("plugin");
    if (!m_pluginPathList.contains(id)) {
        qWarning(plugin) << "Plugin ID" << id << "not found, could not be enabled";
        return false;
    }
    if (m_enabledPlugins.contains(id)) {
        qInfo() << "Plugin ID" << id << "already enabled";
        if (priorList.empty()) refreshPluginTable();
        return true;
    }
    QPluginLoader *loader = new QPluginLoader(m_pluginPathList.value(id));
    if (!loader->load()) {
        qWarning(plugin) << "Plugin" << loader->fileName() << "could not be loaded due to" << loader->errorString();
        loader->deleteLater();
        return false;
    }
    KijangPlugin* kijangPlugin = qobject_cast<KijangPlugin*>(loader->instance());
    if (!kijangPlugin) {
        qWarning(plugin) << "Plugin" << loader->fileName() << "could not be cast to a KijangPlugin instance";
        loader->unload();
        loader->deleteLater();
        return false;
    }
    KijangPluginMetadata pluginMetadata = kijangPlugin->metadata();
    // Check if plugin ID matches real ID
    if (pluginMetadata.pluginID != id) {
        qWarning(plugin) << "Plugin" << loader->fileName() << "has an ID of" << pluginMetadata.pluginID << "instead of the expected ID" << id;
        loader->unload();
        loader->deleteLater();
        return false;
    }
    // Check if plugin has any incompatible plugins enabled
    for (int i = 0; i < pluginMetadata.incompatiblePlugins.length(); i++) {
        if (m_enabledPlugins.contains(pluginMetadata.incompatiblePlugins.at(i))) {
            KijangPluginMetadata currentMetadata = m_enabledPlugins.value(pluginMetadata.incompatiblePlugins.at(i));
            qWarning(plugin) << "Incompatible plugin" << currentMetadata.pluginName << "( ID" << currentMetadata.pluginID << ") conflicting with plugin" << pluginMetadata.pluginName << "( ID" << id << ")";
            loader->unload();
            loader->deleteLater();
            return false;
        }
    }
    // Check if dependencies are enabled
    for (int i = 0; i < pluginMetadata.dependencies.length(); i++) {
        QString currentID = pluginMetadata.dependencies.at(i);
        if (!m_enabledPlugins.contains(currentID)) {
            if (enableDependencies) {
                // Recursive enabling
                QPair<QString, QString> currentRelation(id, currentID); // Dependant, dependency
                if (priorList.contains(currentRelation)) {
                    qWarning(plugin) << "Circular dependency detected between" << id << "and" << currentID;
                    return false;
                }

                priorList.append(currentRelation);
                if (!enablePlugin(currentID, true, priorList)) {
                    qWarning(plugin) << "Dependency" << pluginMetadata.dependencies.at(i) << "unable to be enabled, plugin ID" << id << "will not be enabled";
                    loader->unload();
                    loader->deleteLater();
                    return false;
                }
            } else {
                qWarning(plugin) << "Dependency" << pluginMetadata.dependencies.at(i) << "disabled for plugin ID" << pluginMetadata.pluginID;
                loader->unload();
                loader->deleteLater();
                return false;
            }
        }
    }

    KijangPluginWrapper *wrapper = new KijangPluginWrapper(this);
    wrapper->setPlugin(id);
    connectWrapperFunctions(wrapper);
    kijangPlugin->setWrapper(wrapper);
    m_pluginObjectList.insert(id, kijangPlugin);
    m_wrapperList.insert(id, wrapper);
    m_loaderList.insert(id, loader);
    m_enabledPlugins.insert(id, pluginMetadata);
    m_disabledPlugins.remove(id);
    if (priorList.empty()) refreshPluginTable();
    emit pluginAdded(id, kijangPlugin->metadata());
    qInfo(plugin) << "Plugin" << id << "successfully enabled";
    return true;
}

bool KijangPluginManager::disablePlugin(QString id, bool disableDependants, QList<QPair<QString, QString> > priorList)
{
    QLoggingCategory plugin("plugin");
    if (!m_pluginPathList.contains(id)) {
        qWarning(plugin) << "Plugin ID" << id << "not found, could not be disabled";
        return false;
    }

    if (!m_enabledPlugins.contains(id)) {
        qInfo(plugin) << "Plugin" << id << "already disabled";
        if (priorList.empty()) refreshPluginTable();
        return true;
    }

    QList<QString> keys = m_enabledPlugins.keys();
    for (int i = 0; i < keys.size(); i++) {
        KijangPluginMetadata metadata = m_enabledPlugins.value(keys.at(i));
        if (metadata.dependencies.contains(keys.at(i))) {
            if (disableDependants) {
                QPair<QString, QString> currentRelation(keys.at(i), id); // Dependant, dependency
                if (!priorList.contains(currentRelation)) { // Ignore for circular dependencies
                    priorList.append(currentRelation);
                    if (!disablePlugin(keys.at(i), true, priorList)) {
                        qWarning(plugin) << "Unable to disable dependant" << keys.at(i) << ", plugin ID" << id << "will not be disabled";
                        return false;
                    }
                }
            } else {
                qWarning(plugin) << "Plugin ID" << keys.at(i) << "is dependant on plugin" << id << ", could not be disabled";
                return false;
            }
        }
    }
    // Disable plugin
    disablePluginAfterConfirmation(id);
    if (priorList.empty()) refreshPluginTable();
    qInfo(plugin) << "Plugin" << id << "successfully disabled";
    return true;
}

bool KijangPluginManager::deletePlugin(QString id, bool disableDependants, bool deleteEnabled)
{
    QLoggingCategory plugin("plugin");
    if (!m_pluginPathList.contains(id)) {
        qCritical(plugin) << "Plugin" << id << "is not in list, this should not occur";
        return false;
    }
    if (m_enabledPlugins.contains(id)) {
        if (!deleteEnabled) {
            qWarning(plugin) << "Plugin" << id << "is still enabled, disable first before deleting";
            return false;
        }
        if (!disablePlugin(id, disableDependants)) {
            qWarning(plugin) << "Plugin" << id << "could not be disabled, unable to delete";
            return false;
        }
    }

    QFile pluginFile(m_pluginPathList.value(id));
    if (!pluginFile.exists()) {
        qWarning(plugin) << "File for plugin" << id << "does not exist, plugin marked as deleted";
        if (m_disabledPlugins.contains(id)) m_disabledPlugins.remove(id);
        refreshPluginTable();
        return true;
    }

    if (pluginFile.remove()) {
        qInfo(plugin) << "Plugin" << id << "removed";
        if (m_disabledPlugins.contains(id)) m_disabledPlugins.remove(id);
        refreshPluginTable();
        return true;
    } else {
        qWarning(plugin) << "Plugin" << id << "could not be deleted due to error" << pluginFile.errorString();
        return false;
    }
}

const QMap<QString, KijangPluginWrapper *> &KijangPluginManager::wrapperList() const
{
    return m_wrapperList;
}

const QMap<QString, QPluginLoader *> &KijangPluginManager::loaderList() const
{
    return m_loaderList;
}

const QMap<QString, KijangPluginMetadata> &KijangPluginManager::enabledPlugins() const
{
    return m_enabledPlugins;
}

const QMap<QString, KijangPluginMetadata> &KijangPluginManager::disabledPlugins() const
{
    return m_disabledPlugins;
}

bool KijangPluginManager::pluginInStartList(QString plugin)
{
    for (int i = 0; i < startEnabledPlugins.length(); i++) {
        if (startEnabledPlugins.at(i) == plugin) {
            return true;
        }
    }
    return false;
}

void KijangPluginManager::setNetworkManager(KijangNetworkManager *newNetworkManager)
{
    m_networkManager = newNetworkManager;
}

void KijangPluginManager::setInputManager(KijangInputManager *newInputManager)
{
    m_inputManager = newInputManager;
}

const QMap<QString, KijangPlugin *> &KijangPluginManager::pluginObjectList() const
{
    return m_pluginObjectList;
}

void KijangPluginManager::connectWrapperFunctions(KijangPluginWrapper *wrapper)
{
    connect(wrapper, &KijangPluginWrapper::forwardAudioInputAdded, this, &KijangPluginManager::forwardAudioInputAdded);
    connect(wrapper, &KijangPluginWrapper::forwardAudioInputRemoved, this, &KijangPluginManager::forwardAudioInputRemoved);
    connect(wrapper, &KijangPluginWrapper::forwardVideoInputAdded, this, &KijangPluginManager::forwardVideoInputAdded);
    connect(wrapper, &KijangPluginWrapper::forwardVideoInputRemoved, this, &KijangPluginManager::forwardVideoInputRemoved);
    connect(wrapper, &KijangPluginWrapper::forwardMotionInputAdded, this, &KijangPluginManager::forwardMotionInputAdded);
    connect(wrapper, &KijangPluginWrapper::forwardMotionInputRemoved, this, &KijangPluginManager::forwardMotionInputRemoved);

    connect(wrapper, &KijangPluginWrapper::forwardUdpListenerInterfaceAdded, this, &KijangPluginManager::forwardUdpListenerInterfaceAdded);
    connect(wrapper, &KijangPluginWrapper::forwardUdpListenerInterfaceRemoved, this, &KijangPluginManager::forwardUdpListenerInterfaceRemoved);
    connect(wrapper, &KijangPluginWrapper::forwardModuleHandlerAdded, this, &KijangPluginManager::forwardModuleHandlerAdded);
    connect(wrapper, &KijangPluginWrapper::forwardModuleHandlerRemoved, this, &KijangPluginManager::forwardModuleHandlerRemoved);
}

// https://www.techiedelight.com/check-given-digraph-dag-directed-acyclic-graph-not/
void KijangPluginManager::dagDfsCheck(QString v, QMap<QString, bool> &discovered, QMap<QString, int> &departure, QStringList &listToDisable, int &time, bool &missingChild)
{
    // mark the current node as discovered
    discovered.insert(v, true);
    KijangPluginMetadata metadata = m_enabledPlugins.value(v);
    bool childMissingDependencies = false;

    if (metadata.pluginID.size() > 0) {
        // do for every edge `v —> u`
        for (const QString &u: metadata.dependencies)
        {
            if (listToDisable.contains(u)) {
                missingChild = true;
                listToDisable.append(v);
            } else if (!discovered.contains(u)) {
                // if `u` is not yet discovered
                dagDfsCheck(u, discovered, departure, listToDisable, time, childMissingDependencies);
            }
        }
    } else {
        // Missing dependency, mark plugin as to be deleted
        missingChild = true;
        listToDisable.append(v);
    }

    // ready to backtrack
    // set departure time of vertex `v`
    departure.insert(v, time++);

    // No missing dependencies but child has missing dependencies
    if (childMissingDependencies) {
        missingChild = true;
        listToDisable.append(v);
    }
}

void KijangPluginManager::disablePluginAfterConfirmation(QString id)
{
    m_pluginObjectList.remove(id);
    m_wrapperList.value(id)->disconnect();
    m_wrapperList.value(id)->deleteLater();
    m_wrapperList.remove(id);
    m_loaderList.value(id)->unload();
    m_loaderList.value(id)->deleteLater();
    m_loaderList.remove(id);
    emit pluginRemoved(id, m_enabledPlugins.value(id));
    m_disabledPlugins.insert(id, m_enabledPlugins.value(id));
    m_enabledPlugins.remove(id);
}

const QMap<QString, QString> &KijangPluginManager::pluginPathList() const
{
    return m_pluginPathList;
}

void KijangPluginManager::forwardAudioInputAdded(QString src, AudioInput *input) {
    // TODO: Handle by manager
    emit pluginAudioInputAdded(src, input);
}

void KijangPluginManager::forwardAudioInputRemoved(QString src, AudioInput *input) {
    // TODO: Handle by manager
    emit pluginAudioInputRemoved(src, input);
}

void KijangPluginManager::forwardVideoInputAdded(QString src, VideoInput *input) {
    // TODO: Handle by manager
    emit pluginVideoInputAdded(src, input);
}

void KijangPluginManager::forwardVideoInputRemoved(QString src, VideoInput *input) {
    // TODO: Handle by manager
    emit pluginVideoInputRemoved(src, input);
}

void KijangPluginManager::forwardMotionInputAdded(QString src, MotionInput *input) {
    // TODO: Handle by manager
    emit pluginMotionInputAdded(src, input);
}

void KijangPluginManager::forwardMotionInputRemoved(QString src, MotionInput *input) {
    // TODO: Handle by manager
    emit pluginMotionInputRemoved(src, input);
}

void KijangPluginManager::forwardUdpListenerInterfaceAdded(QString src, UdpListenerInterface *interface) {
    // TODO: Handle by manager
    emit pluginUdpListenerInterfaceAdded(src, interface);
}

void KijangPluginManager::forwardUdpListenerInterfaceRemoved(QString src, UdpListenerInterface *interface) {
    // TODO: Handle by manager
    emit pluginUdpListenerInterfaceRemoved(src, interface);
}

void KijangPluginManager::forwardModuleHandlerAdded(QString src, KijangModuleHandler *handler) {
    m_networkManager->addModule(handler);
    emit pluginModuleHandlerAdded(src, handler);
}

void KijangPluginManager::forwardModuleHandlerRemoved(QString src, KijangModuleHandler *handler) {
    m_networkManager->removeModule(handler->module());
    emit pluginModuleHandlerRemoved(src, handler);
}

void KijangPluginManager::forwardRequestAllMetadata(QString src) {
    if (!m_enabledPlugins.contains(src)) return;
    m_pluginObjectList.value(src)->allMetadata(m_enabledPlugins);
}

void KijangPluginManager::forwardRequestAllPlugins(QString src) {
    if (!m_enabledPlugins.contains(src)) return;
    m_pluginObjectList.value(src)->allObjects(m_pluginObjectList);
}

void KijangPluginManager::forwardRequestAllAudioInput(QString src) {
    if (!m_enabledPlugins.contains(src)) return;
    QMap<QString, QList<AudioInput *>> response;
    for (int i = 0; i < m_pluginObjectList.size(); i++) {
        KijangPlugin *currentPlugin = m_pluginObjectList.value(src);
        QString id = currentPlugin->metadata().pluginID;
        if (src == id) continue;
        response.insert(id, currentPlugin->audioInputs());
    }
    m_pluginObjectList.value(src)->allAudioInputs(response);
}

void KijangPluginManager::forwardRequestAllMotionInput(QString src) {
    if (!m_enabledPlugins.contains(src)) return;
    QMap<QString, QList<MotionInput *>> response;
    for (int i = 0; i < m_pluginObjectList.size(); i++) {
        KijangPlugin *currentPlugin = m_pluginObjectList.value(src);
        QString id = currentPlugin->metadata().pluginID;
        if (src == id) continue;
        response.insert(id, currentPlugin->motionInputs());
    }
    m_pluginObjectList.value(src)->allMotionInputs(response);
}

void KijangPluginManager::forwardRequestAllVideoInput(QString src) {
    if (!m_enabledPlugins.contains(src)) return;
    QMap<QString, QList<VideoInput *>> response;
    for (int i = 0; i < m_pluginObjectList.size(); i++) {
        KijangPlugin *currentPlugin = m_pluginObjectList.value(src);
        QString id = currentPlugin->metadata().pluginID;
        if (src == id) continue;
        response.insert(id, currentPlugin->videoInputs());
    }
    m_pluginObjectList.value(src)->allVideoInputs(response);
}

void KijangPluginManager::forwardRequestAllUdpListener(QString src) {
    if (!m_enabledPlugins.contains(src)) return;
    QMap<QString, QList<UdpListenerInterface *>> response;
    for (int i = 0; i < m_pluginObjectList.size(); i++) {
        KijangPlugin *currentPlugin = m_pluginObjectList.value(src);
        QString id = currentPlugin->metadata().pluginID;
        if (src == id) continue;
        response.insert(id, currentPlugin->udpListeners());
    }
    m_pluginObjectList.value(src)->allUdpListenerInterfaces(response);
}

void KijangPluginManager::forwardRequestAllModuleHandlers(QString src) {
    if (!m_enabledPlugins.contains(src)) return;
    QMap<QString, QList<KijangModuleHandler *>> response;
    for (int i = 0; i < m_pluginObjectList.size(); i++) {
        KijangPlugin *currentPlugin = m_pluginObjectList.value(src);
        QString id = currentPlugin->metadata().pluginID;
        if (src == id) continue;
        response.insert(id, currentPlugin->moduleHandlers());
    }
    m_pluginObjectList.value(src)->allModuleHandlers(response);
}

void KijangPluginManager::forwardRequestPluginMetadata(QString src, QString plugin) {
    if (!m_enabledPlugins.contains(src)) return;
    if (!m_enabledPlugins.contains(plugin)) m_pluginObjectList.value(src)->pluginNonExistent(plugin);
    m_pluginObjectList.value(src)->pluginMetadata(plugin, m_pluginObjectList.value(plugin)->metadata());
}

void KijangPluginManager::forwardRequestPluginAudioInput(QString src, QString plugin) {
    if (!m_enabledPlugins.contains(src)) return;
    if (!m_enabledPlugins.contains(plugin)) m_pluginObjectList.value(src)->pluginNonExistent(plugin);
    m_pluginObjectList.value(src)->pluginAudioInputs(plugin, m_pluginObjectList.value(plugin)->audioInputs());
}

void KijangPluginManager::forwardRequestPluginMotionInput(QString src, QString plugin) {
    if (!m_enabledPlugins.contains(src)) return;
    if (!m_enabledPlugins.contains(plugin)) m_pluginObjectList.value(src)->pluginNonExistent(plugin);
    m_pluginObjectList.value(src)->pluginMotionInputs(plugin, m_pluginObjectList.value(plugin)->motionInputs());
}

void KijangPluginManager::forwardRequestPluginVideoInput(QString src, QString plugin) {
    if (!m_enabledPlugins.contains(src)) return;
    if (!m_enabledPlugins.contains(plugin)) m_pluginObjectList.value(src)->pluginNonExistent(plugin);
    m_pluginObjectList.value(src)->pluginVideoInputs(plugin, m_pluginObjectList.value(plugin)->videoInputs());
}

void KijangPluginManager::forwardRequestPluginUdpListener(QString src, QString plugin) {
    if (!m_enabledPlugins.contains(src)) return;
    if (!m_enabledPlugins.contains(plugin)) m_pluginObjectList.value(src)->pluginNonExistent(plugin);
    m_pluginObjectList.value(src)->pluginUdpListenerInterfaces(plugin, m_pluginObjectList.value(plugin)->udpListeners());
}

void KijangPluginManager::forwardRequestPluginModuleHandlers(QString src, QString plugin) {
    if (!m_enabledPlugins.contains(src)) return;
    if (!m_enabledPlugins.contains(plugin)) m_pluginObjectList.value(src)->pluginNonExistent(plugin);
    m_pluginObjectList.value(src)->pluginModuleHandlers(plugin, m_pluginObjectList.value(plugin)->moduleHandlers());
}

void KijangPluginManager::forwardEventSignal(QString src, QList<QVariant> values) {
    emit pluginEvent(src, values);
}

void KijangPluginManager::forwardFatalSignal(QString src, QString error) {
    emit pluginFatal(src, error);
    QLoggingCategory plugin("plugin");
    if (disablePlugin(src, true)) {
        qInfo(plugin) << "Plugin" << src << "disabled after fatal error" << error << "emitted";
        QWidget tempWidget;
        QMessageBox::critical(&tempWidget, "Plugin error", "An error has occurred in the plugin " + src + " and it has been disabled. Please check the logs for more information.");
    } else {
        qCritical(plugin) << "Plugin" << src << "could not be disabled after error" << error << "emitted, plugins may not work as expected";
        QWidget tempWidget;
        QMessageBox::critical(&tempWidget, "Plugin error", "An error has occurred in the plugin " + src + " but it could not be disabled. The application may not function as intended.");
    }
}
