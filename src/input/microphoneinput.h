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
    Q_INTERFACES(AudioInput)
public:
    explicit MicrophoneInput(QObject *parent = nullptr);
    ~MicrophoneInput();
    void setDeviceInfo(const QAudioDevice &newDeviceInfo, QAudioFormat::SampleFormat sampleFormat);
    static QAudioFormat standardFormat();
    static QAudioFormat::SampleFormat isDeviceCompatible(QAudioDevice &device);

private:
    QAudioDevice m_deviceInfo;
    QAudioSource* m_audioSource;
    bool m_audioStarted;
    bool m_audioErrorred;
    QString m_audioErrorString;
    QBuffer m_audioStream;

signals:


    // AudioInput interface
public:
    void audioStart() Q_DECL_OVERRIDE;
    void audioStop() Q_DECL_OVERRIDE;
    const QString &audioErrorString() const Q_DECL_OVERRIDE;
    bool audioStarted() const Q_DECL_OVERRIDE;
    bool audioErrorred() const Q_DECL_OVERRIDE;
    const QBuffer &audioStream() const Q_DECL_OVERRIDE;

public slots:
    void stateChanged(QAudio::State state);

};

#endif // MICROPHONEINPUT_H
