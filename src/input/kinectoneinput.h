#ifndef KINECTONEINPUT_H
#define KINECTONEINPUT_H

#include <QObject>
#include "audioinput.h"
#include "videoinput.h"
#include "motioninput.h"

class KinectOneInput : public QObject, public AudioInput, public VideoInput, public MotionInput
{
    Q_OBJECT
public:
    explicit KinectOneInput(QObject *parent = nullptr);

signals:

};

#endif // KINECTONEINPUT_H
