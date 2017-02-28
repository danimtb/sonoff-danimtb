#pragma once

//Type of timer to define
#define PULSE 0                             // One pulse timer
#define RT_ON 1                             // Retarded to connection timer
#define RT_OFF 2                            // Retarded to disconnection timer

#include "Arduino.h"

class SimpleTimer
{
private:
    unsigned long m_currentMillis;          // Time when the timer is loaded
    unsigned long m_milliSeconds;           // Time period of your timer
    int m_type;

public:
    SimpleTimer();
    void setup(unsigned int type);                              // Set up type of timer: PULSE, RT_ON, RT_OFF
    void setup(unsigned int type, unsigned long milliseconds);  // Set up type of timer (PULSE, RT_ON, RT_OFF) and time
    void load(unsigned long milliSeconds);                      // Load timer with time
    void start();                                               // Start timer counting time
    bool check();                                               // Check if time is over (loop)
};
