#ifndef KIJANGLOGGER_H
#define KIJANGLOGGER_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <iostream>
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
    void logStringChanged();

public slots:
    static void flush();

private:
    static QString filename;
    static QStringList cachedLogs;
    static int cacheCount;
    static int exportCacheCount;
    static qint64 lastExportTime;
    static int minExportInterval;
    static QString m_logString;

signals:

};

#endif // KIJANGLOGGER_H
