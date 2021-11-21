#ifndef VIDEOINPUT_H
#define VIDEOINPUT_H

#include <QtPlugin>

class VideoInput
{
    Q_DISABLE_COPY(VideoInput)
public:
    explicit VideoInput() = default;
    virtual ~VideoInput() {}

};

Q_DECLARE_INTERFACE(VideoInput, "moe.kancil.kijang.input.videoinput")

#endif // VIDEOINPUT_H
