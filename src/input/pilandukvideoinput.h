#ifndef PILANDUKVIDEOINPUT_H
#define PILANDUKVIDEOINPUT_H

#include <QObject>
#include "videoinput.h"

class PilandukVideoInput : public QObject, public VideoInput
{
    Q_OBJECT
public:
    explicit PilandukVideoInput(QObject *parent = nullptr);

signals:

};

#endif // PILANDUKVIDEOINPUT_H
