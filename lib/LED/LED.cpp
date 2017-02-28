
#include "LED.h"

void LED::setup(int pin, int type)
{
	m_pin = pin;
	pinMode(m_pin, OUTPUT);
	m_type = type;
	m_state = false;
	m_blinkPeriod = 0;
	m_blinkTimer.setup(RT_ON);
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

void LED::blink(unsigned long blinkPeriod)
{
	if (m_blinkPeriod != blinkPeriod)
	{
		m_blinkPeriod = blinkPeriod;

		this->on();
        m_blinkTimer.load(m_blinkPeriod/2);
        m_blinkTimer.start();
	}
	else
	{
		if (m_blinkTimer.check())
		{
			this->commute();
            m_blinkTimer.start();
		}
	}
}
