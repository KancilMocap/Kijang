#ifndef PILANDUKMOTIONINPUT_H
#define PILANDUKMOTIONINPUT_H

#include <QObject>
#include "motioninput.h"

class PilandukMotionInput : public QObject, public MotionInput
{
    Q_OBJECT
    Q_INTERFACES(MotionInput)
public:
    explicit PilandukMotionInput(QObject *parent = nullptr);

signals:

};

#endif // PILANDUKMOTIONINPUT_H
