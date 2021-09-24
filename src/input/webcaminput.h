#ifndef WEBCAMINPUT_H
#define WEBCAMINPUT_H

#include <QObject>
#include "videoinput.h"
#include "audioinput.h"

class WebcamInput : public QObject, public AudioInput, public VideoInput
{
    Q_OBJECT
    Q_INTERFACES(AudioInput VideoInput)
public:
    explicit WebcamInput(QObject *parent = nullptr);

signals:

};

#endif // WEBCAMINPUT_H
