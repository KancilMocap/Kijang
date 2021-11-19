#ifndef KIJANGLOGGER_H
#define KIJANGLOGGER_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <iostream>
#include <QTimer>
#include <QTextStream>

class KijangLogger : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString logString READ logString WRITE setLogString NOTIFY logStringChanged)
public:
    explicit KijangLogger(QObject *parent = nullptr);
    static void attach();
    static void handler(QtMsgType type, const QMessageLogContext &context, const QString & msg);

    static const QString &logString();
    static void setLogString(const QString &newLogString);

signals:
    void logStringChanged(const QString &newText);

public slots:
    static void flush();

private:
    static QString rootDir;
    static QString filename;
    static QStringList cachedLogs;
    static int cacheCount;
    static int exportCacheCount;
    static qint64 lastExportTime;
    static int minExportInterval;
    static QString m_logString;

    QTimer *timer;
    void updateLogUI();

signals:

};

#endif // KIJANGLOGGER_H
