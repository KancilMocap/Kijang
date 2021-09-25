#include "kijangpluginmanager.h"

QString KijangPluginManager::pluginDirPath = QDir::currentPath() + QDir::separator() + "plugins";
QString KijangPluginManager::pluginDataPath = QDir::currentPath() + QDir::separator() + "plugins" + QDir::separator() + "enabled.dat";

KijangPluginManager::KijangPluginManager(QObject *parent) : QObject(parent)
{
    QLoggingCategory plugin("plugin");
    qDebug(plugin) << "Plugin manager constructed";

    // Load enabled plugins
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
        QErrorMessage errorBox;
        errorBox.showMessage("Sorry! Your operating system is not supported. Plugins could not be imported.");
        return;
    }

    QString filePath = QFileDialog::getOpenFileName(nullptr, "Open file", QDir::homePath(), libraryFiles);
    if (filePath.isNull()) return;

    QPluginLoader *loader = new QPluginLoader(filePath);
    if (!loader->load()) {
        qWarning(plugin) << "Invalid library file" << filePath << ", unable to import plugin";
        // FIXME: QErrorMessage not showing
        QErrorMessage errorBox;
        errorBox.showMessage("The plugin library appears to be invalid.");
        return;
    }

    KijangPlugin* kijangPlugin = qobject_cast<KijangPlugin*>(loader->instance());
    if (!kijangPlugin) {
        qWarning(plugin) << "Library file" << filePath << "could not be cast to a KijangPlugin instance, unable to import plugin";
        // FIXME: QErrorMessage not showing
        QErrorMessage errorBox;
        errorBox.showMessage("The plugin library appears to be invalid.");
        return;
    }

    KijangPluginMetadata pluginMetadata = kijangPlugin->metadata();
    QString loaderName = loader->fileName();
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
    QMessageBox msgBox;
    msgBox.setText(title);
    msgBox.setInformativeText(metadataInfo.join("\n"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if (ret | QMessageBox::Yes) {
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
                // Direct enable without going through enablePlugin, might cause weird issues
                KijangPluginWrapper *wrapper = new KijangPluginWrapper(this);
                wrapper->setPlugin(kijangPlugin);
                m_enabledPlugins.insert(pluginMetadata.pluginID, pluginMetadata);
                m_loaderList.insert(pluginMetadata.pluginID, loader);
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

    // TODO: Check if required dependencies enabled
    qInfo(plugin) << pluginFileCount << "plugins found," << pluginCount << "successfully loaded";
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
    wrapper->setPlugin(kijangPlugin);
    m_wrapperList.insert(id, wrapper);
    m_loaderList.insert(id, loader);
    m_enabledPlugins.insert(id, pluginMetadata);
    m_disabledPlugins.remove(id);
    if (priorList.empty()) refreshPluginTable();
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
    m_wrapperList.value(id)->deleteLater();
    m_wrapperList.remove(id);
    m_loaderList.value(id)->unload();
    m_loaderList.value(id)->deleteLater();
    m_loaderList.remove(id);
    m_disabledPlugins.insert(id, m_enabledPlugins.value(id));
    m_enabledPlugins.remove(id);
    if (priorList.empty()) refreshPluginTable();
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

const QMap<QString, QString> &KijangPluginManager::pluginPathList() const
{
    return m_pluginPathList;
}

