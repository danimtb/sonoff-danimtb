
#include "Relay.h"

void Relay::setup(int pin, int type)
{
	m_pin = pin;
	pinMode(m_pin, OUTPUT);
	m_type = type;
	m_state = false;
}

void Relay::on()
{
	if (m_type == RELAY_HIGH_LVL)
	{
		digitalWrite(m_pin, HIGH);
	}

	if (m_type == RELAY_LOW_LVL)
	{
		digitalWrite(m_pin, LOW);
	}

	m_state = true;
}

void Relay::off()
{
	if (m_type == RELAY_HIGH_LVL)
	{
		digitalWrite(m_pin, LOW);
	}

	if (m_type == RELAY_LOW_LVL)
	{
		digitalWrite(m_pin, HIGH);
	}

	m_state = false;
}

bool Relay::getState()
{
	return m_state;
}

void Relay::commute()
{
	if(m_state)
	{
		off();
	}
	else
	{
		on();
	}
}
