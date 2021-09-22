#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDateTime>
#include <QWindow>
#include "application/kijangapp.h"
#include "application/kijanglogger.h"

int main(int argc, char *argv[])
{
    KijangApp kijang;
    int response = kijang.run(argc, argv);
    qDebug() << "Response code: " << response;
    KijangLogger::flush();
    return response;
}
