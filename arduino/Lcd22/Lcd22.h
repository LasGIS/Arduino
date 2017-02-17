#ifndef LCD22_h
#define LCD22_h

#include <Arduino.h>

class LgMeasure {
public:
    char* description;
    uint8_t pin;
    uint16_t color;
    float factor;
    LgMeasure(const char* _description, uint8_t _pin,
              uint16_t _color, float _value);
};

#endif