#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QString>
#include <QBuffer>

class AudioInput
{
public:
    AudioInput();
    virtual void audioStart() = 0;
    virtual void audioStop() = 0;
    const QString &audioErrorString() const;

    bool audioStarted() const;
    bool audioErrorred() const;

    // To allow multiple inheritance between AudioInput, VideoInput and MotionInput, Qt's signals and slots could not be used
    // Custom signals used to relay to slots instead
    const QBuffer &audioStream() const;
    // Each buffer write follows "S:___" where S would be a letter indicating the signal type"
    const QBuffer &audioCustomSignals() const;

protected:
    bool m_audioStarted;
    bool m_audioErrorred;
    QString m_audioErrorString;
    QBuffer m_audioStream;
    QBuffer m_audioCustomSignals;

};

#endif // AUDIOINPUT_H
