#ifndef KIJANGPLUGINWRAPPER_H
#define KIJANGPLUGINWRAPPER_H

#include <QObject>

class KijangPluginWrapper : public QObject
{
    Q_OBJECT
public:
    explicit KijangPluginWrapper(QObject *parent = nullptr);

signals:

};

#endif // KIJANGPLUGINWRAPPER_H
