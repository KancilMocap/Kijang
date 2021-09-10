#ifndef PILANDUKAUDIOINPUT_H
#define PILANDUKAUDIOINPUT_H

#include <QObject>
#include "audioinput.h"

class PilandukAudioInput : public QObject, public AudioInput
{
    Q_OBJECT
public:
    explicit PilandukAudioInput(QObject *parent = nullptr);

signals:

};

#endif // PILANDUKAUDIOINPUT_H
