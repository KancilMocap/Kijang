#include "kijanginputmanager.h"

Q_DECLARE_LOGGING_CATEGORY(inputs);
Q_LOGGING_CATEGORY(inputs,"inputs");

KijangInputManager::KijangInputManager(QObject *parent) : QObject(parent)
{
    qDebug(inputs) << "Input manager constructed";
}

KijangInputManager::~KijangInputManager()
{
    qDebug(inputs) << "Input manager deconstructed";
}

void KijangInputManager::start()
{
    // Initialise audio devices
    QList<QAudioDevice> deviceInfos = QMediaDevices::audioInputs();
    for(int i = 0; i < deviceInfos.length(); i++) {
        QAudioDevice currentDevice = deviceInfos.at(i);
        qDebug(inputs) << "Found audio device of ID" << currentDevice.id() << "with description" << currentDevice.description();
        QAudioFormat::SampleFormat sampleFormat = MicrophoneInput::isDeviceCompatible(currentDevice);
        if (sampleFormat != QAudioFormat::Unknown) {
            MicrophoneInput *audioInput = new MicrophoneInput(this);
            audioInput->setDeviceInfo(currentDevice, sampleFormat);
            m_audioList.append(audioInput);
            audioInput->audioStart();
            // TODO: Update UI to add audio source
            qDebug(inputs) << "Audio device successfully added";
        } else {
            qDebug(inputs) << "Unable to detect supported audio format, skipping";
        }
    }

    // TODO: Initialise video devices

    // TODO: Initialise motion devices
}

void KijangInputManager::stop()
{

}
