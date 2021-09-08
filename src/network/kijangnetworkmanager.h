#ifndef KIJANGNETWORKMANAGER_H
#define KIJANGNETWORKMANAGER_H

#include <QObject>
#include <QDebug>

class KijangNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit KijangNetworkManager(QObject *parent = nullptr);

signals:

};

#endif // KIJANGNETWORKMANAGER_H
