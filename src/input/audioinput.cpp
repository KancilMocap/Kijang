#include "audioinput.h"

AudioInput::AudioInput()
{

}

bool AudioInput::audioStarted() const
{
    return m_audioStarted;
}

bool AudioInput::audioErrorred() const
{
    return m_audioErrorred;
}

const QBuffer &AudioInput::audioStream() const
{
    return m_audioStream;
}

const QBuffer &AudioInput::audioCustomSignals() const
{
    return m_audioCustomSignals;
}

const QString &AudioInput::audioErrorString() const
{
    return m_audioErrorString;
}
