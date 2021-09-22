#ifndef KIJANGPLUGINMANAGER_H
#define KIJANGPLUGINMANAGER_H

#include <QObject>

class KijangPluginManager : public QObject
{
    Q_OBJECT
public:
    explicit KijangPluginManager(QObject *parent = nullptr);

signals:

};

#endif // KIJANGPLUGINMANAGER_H
