#include "microphoneinput.h"

MicrophoneInput::MicrophoneInput(QObject *parent) : QObject(parent)
{
    QLoggingCategory inputs("inputs");
    qDebug(inputs) << "Microphone input constructed";
}

MicrophoneInput::~MicrophoneInput()
{
    QLoggingCategory inputs("inputs");
    qDebug(inputs) << "Microphone input deconstructed";
}

void MicrophoneInput::setDeviceInfo(const QAudioDevice &newDeviceInfo, QAudioFormat::SampleFormat sampleFormat)
{
    m_deviceInfo = newDeviceInfo;
    // Creates QAudioSource from device info
    QAudioFormat format;
    format.setSampleFormat(sampleFormat);
    m_audioSource = new QAudioSource(m_deviceInfo, format, this);
    connect(m_audioSource, &QAudioSource::stateChanged, this, &MicrophoneInput::stateChanged);
}

QAudioFormat MicrophoneInput::standardFormat()
{
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Float);
    return format;
}

QAudioFormat::SampleFormat MicrophoneInput::isDeviceCompatible(QAudioDevice &device)
{
    QAudioFormat format = standardFormat();
    if (device.isFormatSupported(format)) return format.sampleFormat();
    for (int i = 1; i <= 3; i++) {
        format.setSampleFormat(static_cast<QAudioFormat::SampleFormat>(i));
        if (device.isFormatSupported(format)) return format.sampleFormat();
    }
    return QAudioFormat::Unknown;
}

void MicrophoneInput::stateChanged(QAudio::State state)
{
    QByteArray stateArray("S:");
    stateArray.append(QVariant::fromValue(state).toString().toUtf8());
    m_audioCustomSignals.write(stateArray);
    switch(state) {
        case QAudio::State::ActiveState:
            audioStart();
        default:
            audioStop();
    }
}

void MicrophoneInput::audioStart()
{
    m_audioStarted = true;
    m_audioErrorred = false;
    m_audioSource->start(&m_audioStream);
    QLoggingCategory inputs("inputs");
    qDebug(inputs) << this << "started";
}

void MicrophoneInput::audioStop()
{
    m_audioStarted = false;
    m_audioErrorred = false;
    m_audioSource->stop();
    QLoggingCategory inputs("inputs");
    qDebug(inputs) << this << "stopped";
}
