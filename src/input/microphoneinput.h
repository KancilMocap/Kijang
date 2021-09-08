#ifndef MICROPHONEINPUT_H
#define MICROPHONEINPUT_H

#include <QObject>
#include "audioinput.h"

class MicrophoneInput : public QObject, public AudioInput
{
    Q_OBJECT
public:
    explicit MicrophoneInput(QObject *parent = nullptr);

signals:

};

#endif // MICROPHONEINPUT_H
