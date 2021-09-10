#ifndef KIJANGINPUTMANAGER_H
#define KIJANGINPUTMANAGER_H

#include <QObject>
#include <QDebug>
#include <QLoggingCategory>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QAudioSource>
#include "audioinput.h"
#include "videoinput.h"
#include "motioninput.h"
#include "microphoneinput.h"

class KijangInputManager : public QObject
{
    Q_OBJECT
public:
    explicit KijangInputManager(QObject *parent = nullptr);
    ~KijangInputManager();
    void start();
    void stop();

signals:

private:
    // Pointers used instead of class to anticipate implementations of AudioInput, VideoInput and MotionInput using those classes with QObject
    QList<AudioInput *> m_audioList;
    QList<VideoInput *> m_videoList;
    QList<MotionInput *> m_motionList;
};

#endif // KIJANGINPUTMANAGER_H
