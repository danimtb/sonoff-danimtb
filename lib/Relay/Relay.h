#ifndef RELAY_H
#define RELAY_H

#include "Arduino.h"

#define RELAY_HIGH_LVL 1 	//Relay active at high level (5v)
#define RELAY_LOW_LVL 0 	//Relay active at low level (0v)

class Relay
{
private:
	int m_pin;
	int m_type;
	bool m_state;
public:
	void setup(int pin, int type);
	void on();
	void off();
	bool getState();
	void commute();
};

#endif
