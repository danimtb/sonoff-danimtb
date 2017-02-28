#include "SimpleTimer.h"

SimpleTimer::SimpleTimer()
{
    m_milliSeconds = 0;
    m_currentMillis = 0;
    m_type = RT_ON;
}

void SimpleTimer::setup(unsigned int type)
{
    m_type = type;
}

void SimpleTimer::setup(unsigned int type, unsigned long milliseconds)
{
    this->setup(type);
    this->load(milliseconds);
}

void SimpleTimer::load(unsigned long milliSeconds)
{
    m_milliSeconds = milliSeconds;
}

void SimpleTimer::start()
{
    m_currentMillis = millis();
}

bool SimpleTimer::check()
{
    if (m_type == PULSE)
    {
        if ((millis() - m_currentMillis) > (m_milliSeconds - 500) && (millis() - m_currentMillis) < (m_milliSeconds + 500))
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
