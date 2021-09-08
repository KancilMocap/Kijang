#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDateTime>
#include <QWindow>
#include "application/kijang.h"
#include "application/kijanglogger.h"

int main(int argc, char *argv[])
{
    Kijang kijang;
    int response = kijang.run(argc, argv);
    qDebug() << "Response code: " << response;
    KijangLogger::flush();
    return response;
}
