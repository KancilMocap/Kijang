#ifndef KIJANG_H
#define KIJANG_H

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDateTime>
#include <QVersionNumber>
#include <QSharedPointer>
#include "kijanglogger.h"

class Kijang
{
public:
    Kijang();
    ~Kijang();

    enum PageType {
        Home,
        Input,
        Settings
    };
    int run(int argc, char** argv);

private:
    QString getTypeQml(PageType type);
    QSharedPointer<QQmlApplicationEngine> m_engine;

public slots:

};

#endif // KIJANG_H
