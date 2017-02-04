#include "SimpleTimer.h"

void SimpleTimer::setup(unsigned int type)
{
    m_type = type;
    m_milliSeconds = 0;
    m_currentMillis = 0;
}

void SimpleTimer::load(unsigned long milliSeconds)
{
    m_milliSeconds = milliSeconds;
    m_currentMillis = millis();
}

bool SimpleTimer::check()
{
    if (m_type == PULSE)
    {
        if ((millis() - m_currentMillis) > (m_milliSeconds - 1000) && (millis() - m_currentMillis) < (m_milliSeconds + 1000))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    if (m_type == RT_ON)
    {
        if (millis() - m_currentMillis > m_milliSeconds)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    if (m_type == RT_OFF)
    {
        if (millis() - m_currentMillis > m_milliSeconds)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}
