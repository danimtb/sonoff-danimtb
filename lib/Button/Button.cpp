#include "Button.h"

void Button::setup(uint8_t pin, uint8_t mode)
{
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

			if (millis() - m_millisSincePressed > 500)
			{
				  m_longPress = true;
			}
			else if (millis() - m_millisSincePressed > 100)
			{
				m_shortPress = true;
			}
			else
			{
				//Too short to register as a press
			}

			m_millisSincePressed = 0;
		}
}

bool Button::shortPress()
{
	if (m_shortPress)
	{
		m_shortPress = false;
		return true;
	}
	else
	{
		return false;
	}
}

bool Button::longPress()
{
	if (m_longPress)
	{
		m_longPress = false;
		return true;
	}
	else
	{
		return false;
	}
}
