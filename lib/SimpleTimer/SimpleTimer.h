#pragma once

//Type of timer to define
#define PULSE 0 //One pulse timer
#define RT_ON 1 //Retarded to connection timer
#define RT_OFF 2 //Retarded to disconnection timer

#include "Arduino.h"

class SimpleTimer
{
private:
	unsigned long currentMillis; //time when the timer is loaded
	unsigned long time; //time period of your timer
	int type;

public:
	void setup(unsigned int t);       //set up type of timer
	void load(unsigned long seconds); //init timer
	bool check();			  //check if time is over
};
