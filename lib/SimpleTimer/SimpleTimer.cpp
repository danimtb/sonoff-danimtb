#include "SimpleTimer.h"

void SimpleTimer::setup(unsigned int t)
{
	type = t;
	time = 0;
	currentMillis = 0;
}

void SimpleTimer::load(unsigned long seconds)
{
	time = seconds;
	currentMillis = millis();
}

bool SimpleTimer::check()
{
	if (type == PULSE)
	{
		if (millis() - currentMillis > time-1000 && millis() - currentMillis < time + 1000)
		{
		  return true;
		}
		else
		{
		  return false;
		}
	}

	if (type == RT_ON)
	{
		if (millis() - currentMillis > time)
		{
		  return true;
		}
		else
		{
		  return false;
		}
	}

	if (type == RT_OFF)
	{
		if (millis() - currentMillis > time)
		{
		  return false;
		}
		else
		{
		  return true;
		}
	}
}
