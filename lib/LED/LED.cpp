
#include "LED.h"

void LED::setup(int pin, int type)
{
	m_pin = pin;
	pinMode(m_pin, OUTPUT);
	m_type = type;
	m_state = false;
}

void LED::on()
{
	if (m_type == LED_HIGH_LVL)
	{
		digitalWrite(m_pin, HIGH);
	}

	if (m_type == LED_LOW_LVL)
	{
		digitalWrite(m_pin, LOW);
	}

	m_state = true;
}

void LED::off()
{
	if (m_type == LED_HIGH_LVL)
	{
		digitalWrite(m_pin, LOW);
	}

	if (m_type == LED_LOW_LVL)
	{
		digitalWrite(m_pin, HIGH);
	}

	m_state = false;
}

bool LED::getState()
{
	return m_state;
}

void LED::commute()
{
	if (m_state)
	{
		off();
	}
	else
	{
		on();
	}
}
