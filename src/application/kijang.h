#ifndef KIJANG_H
#define KIJANG_H

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDateTime>
#include <QSettings>
#include <QVersionNumber>
#include <QSharedPointer>
#include <QtQml>
#include "kijanglogger.h"
#include "../network/kijangprotocol.h"
#include "../input/kijanginputmanager.h"
#include "../network/kijangnetworkmanager.h"

class Kijang : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ethernetEnabled READ ethernetEnabled WRITE setEthernetEnabled NOTIFY ethernetEnabledChanged)
    Q_PROPERTY(bool serialEnabled READ serialEnabled WRITE setSerialEnabled NOTIFY serialEnabledChanged)
public:
    explicit Kijang(QObject *parent = nullptr);
    ~Kijang();

    int run(int argc, char** argv);

    static const QString &getSettingsFile();
    const KijangInputManager &inputManager() const;
    const KijangNetworkManager &networkManager() const;



    bool ethernetEnabled() const;
    void setEthernetEnabled(bool newEthernetEnabled);

    bool serialEnabled() const;
    void setSerialEnabled(bool newSerialEnabled);

private:
    bool m_ethernetEnabled;
    bool m_serialEnabled;

    static QString settingsFile;
    QSharedPointer<QQmlApplicationEngine> m_engine;

    KijangInputManager m_inputManager;
    KijangNetworkManager m_networkManager;

signals:
    void ethernetEnabledChanged();
    void serialEnabledChanged();

public slots:

};

#endif // KIJANG_H
