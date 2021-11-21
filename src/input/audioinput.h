#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QtPlugin>
#include <QBuffer>

class AudioInput
{
    Q_DISABLE_COPY(AudioInput)
public:
    explicit AudioInput() = default;
    virtual ~AudioInput() {}
    virtual void audioStart() = 0;
    virtual void audioStop() = 0;
    virtual const QString &audioErrorString() const = 0;

    virtual bool audioStarted() const = 0;
    virtual bool audioErrorred() const = 0;

    virtual const QBuffer &audioStream() const = 0;
    // https://stackoverflow.com/a/18113601 - Signals in abstract non-QObject classes
signals:

};

Q_DECLARE_INTERFACE(AudioInput, "moe.kancil.kijang.input.audioinput")

#endif // AUDIOINPUT_H
