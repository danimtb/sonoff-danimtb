#include "Button.h"

void shortPressCallback() {}
void longPressCallback() {}
void longlongPressCallback() {}

void Button::setup(uint8_t pin, uint8_t mode)
{
    m_longlongPressCallback = &longlongPressCallback;
    m_longPressCallback = &longPressCallback;
    m_shortPressCallback = &shortPressCallback;

    m_state = false;
    m_millisSincePressed = 0;
    m_pin = pin;
    m_mode = mode;
    pinMode(m_pin, INPUT);
}

bool Button::isPressed()
{
    if (digitalRead(m_pin) == m_mode)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Button::loop()
{
    if (isPressed() && !m_state)
    {
        //Button has been pressed, store time of press
        m_state = true;
        m_millisSincePressed = millis();
    }
    else if (!isPressed() && m_state)
    {
        //Button has been released, trigger one of the two possible options
        m_state = false;

        if (millis() - m_millisSincePressed > 10000)
        {
            m_longlongPressCallback();
        }
        else if (millis() - m_millisSincePressed > 500)
        {
            m_longPressCallback();
        }
        else if (millis() - m_millisSincePressed > 100)
        {
            m_shortPressCallback();
        }
        else
        {
            //Too short to register as a press
        }

        m_millisSincePressed = 0;
    }
}

void Button::setShortPressCallback(void (*callback)())
{
    m_shortPressCallback = callback;
}

void Button::setLongPressCallback(void (*callback)())
{
    m_longPressCallback = callback;
}

void Button::setLongLongPressCallback(void (*callback)())
{
    m_longlongPressCallback = callback;
}
