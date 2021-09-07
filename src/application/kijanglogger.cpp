#include "kijanglogger.h"

QString KijangLogger::filename = QDir::currentPath() + QDir::separator() + "logs" + QDir::separator() + "kijang.log";
int KijangLogger::cacheCount = 0;
int KijangLogger::exportCacheCount = 200;
qint64 KijangLogger::lastExportTime = QDateTime::currentSecsSinceEpoch();
int KijangLogger::minExportInterval = 10;
QStringList KijangLogger::cachedLogs = QStringList();
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(nullptr);

KijangLogger::KijangLogger(QObject *parent) : QObject(parent)
{

}

void KijangLogger::attach()
{
    // Create logs folder if not exists
    QDir logsFolder(QDir::currentPath() + QDir::separator() + "logs");
    // https://stackoverflow.com/a/11517874
    if (!logsFolder.exists()) {
        logsFolder.mkpath(".");
    }
    qInstallMessageHandler(KijangLogger::handler);
}

void KijangLogger::handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString txt;
    switch (type) {
    case QtInfoMsg:
        txt = QString("[INFO] %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("[WARNING] %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("[CRITICAL] %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("[FATAL] %1").arg(msg);
        break;
    case QtDebugMsg:
        txt = QString("[DEBUG] %1").arg(msg);
        break;
    }
#ifdef QT_DEBUG
    txt = QDateTime::currentDateTime().toString() + " - " + txt + context.file + " line: " + QString::number(context.line);
    KijangLogger::cachedLogs << txt;
    KijangLogger::cacheCount += 1;
#else
    if (type != QtDebugMsg) {
        txt = QDateTime::currentDateTime().toString() + " - " + txt + context.file + " line: " + QString::number(context.line);
        KijangLogger::cachedLogs << txt;
        KijangLogger::cacheCount += 1;
    }
#endif
    qint64 currentTime = QDateTime::currentSecsSinceEpoch();
    if (type == QtCriticalMsg || type == QtFatalMsg || (KijangLogger::cacheCount >= 200 && (currentTime - KijangLogger::lastExportTime > KijangLogger::minExportInterval))) {
        KijangLogger::flush();
    }

    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);

}

void KijangLogger::flush()
{
    QFile file(KijangLogger::filename);
    if(file.open(QIODevice::Append) && KijangLogger::cachedLogs.length() > 0) {
        KijangLogger::lastExportTime = QDateTime::currentSecsSinceEpoch();
        QTextStream ts(&file);
        ts << cachedLogs.join("\r\n") << "\r\n";
        ts.flush();
        file.close();
    }
}
