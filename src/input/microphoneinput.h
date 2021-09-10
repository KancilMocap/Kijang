#ifndef MICROPHONEINPUT_H
#define MICROPHONEINPUT_H

#include <QObject>
#include <QAudioDevice>
#include <QAudioSource>
#include <QLoggingCategory>
#include <QVariant>
#include "audioinput.h"

class MicrophoneInput : public QObject, public AudioInput
{
    Q_OBJECT
public:
    explicit MicrophoneInput(QObject *parent = nullptr);
    ~MicrophoneInput();

private:
    QAudioDevice m_deviceInfo;
    QAudioSource* m_audioSource;

signals:


    // AudioInput interface
public:
    void audioStart() Q_DECL_OVERRIDE;
    void audioStop() Q_DECL_OVERRIDE;
    void setDeviceInfo(const QAudioDevice &newDeviceInfo, QAudioFormat::SampleFormat sampleFormat);
    static QAudioFormat standardFormat();
    static QAudioFormat::SampleFormat isDeviceCompatible(QAudioDevice &device);

public slots:
    void stateChanged(QAudio::State state);
};

#endif // MICROPHONEINPUT_H
