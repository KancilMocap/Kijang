#ifndef KINECT360INPUT_H
#define KINECT360INPUT_H

#include <QObject>
#include "audioinput.h"
#include "videoinput.h"
#include "motioninput.h"

class Kinect360Input : public QObject, public VideoInput, public MotionInput
{
    Q_OBJECT
    Q_INTERFACES(VideoInput MotionInput)
public:
    explicit Kinect360Input(QObject *parent = nullptr);

signals:

};

#endif // KINECT360INPUT_H
