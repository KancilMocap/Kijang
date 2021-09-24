#include "kijangpluginwrapper.h"

KijangPluginWrapper::KijangPluginWrapper(QObject *parent) : QObject(parent)
{

}

void KijangPluginWrapper::setPlugin(KijangPlugin *newPlugin)
{
    m_plugin = newPlugin;
}

KijangPlugin *KijangPluginWrapper::plugin() const
{
    return m_plugin;
}
