#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDateTime>
#include <QWindow>
#include "application/kijang.h"

int main(int argc, char *argv[])
{
    Kijang kijang;
    int response = kijang.run(argc, argv);
    qDebug() << "Response code: " << response;
    return response;
}
