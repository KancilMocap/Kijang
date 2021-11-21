#ifndef MOTIONINPUT_H
#define MOTIONINPUT_H

#include <QtPlugin>

class MotionInput
{
    Q_DISABLE_COPY(MotionInput)
public:
    explicit MotionInput() = default;
    virtual ~MotionInput() {}

signals:

};

Q_DECLARE_INTERFACE(MotionInput, "moe.kancil.kijang.input.motioninput")

#endif // MOTIONINPUT_H
