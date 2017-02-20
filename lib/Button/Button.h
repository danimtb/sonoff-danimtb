#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

#define PULLUP HIGH
#define PULLDOWN LOW

class Button
{
  public:
    void setup(uint8_t pin, uint8_t mode = PULLDOWN);
    bool isPressed();
    void setShortPressCallback(void (*callback)());
    void setLongPressCallback(void (*callback)());
    void setLongLongPressCallback(void (*callback)());
    void loop();

  private:
    uint8_t m_pin;
    uint8_t m_mode;
    uint8_t m_state;
    unsigned long m_millisSincePressed;
    bool m_shortPress;
    bool m_longPress;

    void (*m_shortPressCallback)(void);
    void (*m_longPressCallback)(void);
    void (*m_longlongPressCallback)(void);
};

#endif
