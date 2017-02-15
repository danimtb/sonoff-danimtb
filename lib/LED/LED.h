#ifndef LED_H
#define LED_H

#include "Arduino.h"

#include "SimpleTimer.h"

#define LED_HIGH_LVL 1 	//LED active at high level
#define LED_LOW_LVL 0 	//LED active at low level

class LED
{
private:
	int m_pin;
	int m_type;
	bool m_state;
	unsigned long m_blinkPeriod;
	SimpleTimer m_blinkTimer;

public:
	void setup(int pin, int type);
	void on();
	void off();
	bool getState();
	void commute();
	void blink(unsigned long blinkPeriod);
};

#endif
