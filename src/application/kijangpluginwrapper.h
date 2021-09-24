#ifndef KIJANGPLUGINWRAPPER_H
#define KIJANGPLUGINWRAPPER_H

#include <QObject>
#include "../kijangplugin.h"

class KijangPluginWrapper : public QObject
{
    Q_OBJECT
public:
    explicit KijangPluginWrapper(QObject *parent = nullptr);

    void setPlugin(KijangPlugin *newPlugin);
    KijangPlugin *plugin() const;

signals:

private:
    KijangPlugin *m_plugin;

};

#endif // KIJANGPLUGINWRAPPER_H
