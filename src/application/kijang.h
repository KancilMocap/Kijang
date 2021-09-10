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
#include "../input/kijanginputmanager.h"
#include "../network/kijangnetworkmanager.h"

class Kijang : public QObject
{
    Q_OBJECT
public:
    explicit Kijang(QObject *parent = nullptr);
    ~Kijang();

    int run(int argc, char** argv);

    static const QString &getSettingsFile();
    const KijangInputManager &inputManager() const;
    const KijangNetworkManager &networkManager() const;

private:
    static QString settingsFile;
    QSharedPointer<QQmlApplicationEngine> m_engine;

    KijangInputManager m_inputManager;
    KijangNetworkManager m_networkManager;

signals:

public slots:

};

#endif // KIJANG_H
