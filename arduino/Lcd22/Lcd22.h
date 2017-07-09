#ifndef LCD22_h
#define LCD22_h

#include <Arduino.h>

#define CHARGER_VT_COLOR    0b0000000000011111
#define CHARGER_VG_COLOR    0b0000000000011111
#define BATTERY_VT_COLOR    0b0000000100011111
#define BATTERY_VG_COLOR    0b0000000100011111
#define SUPPLY5_VT_COLOR    0b0000001000011111
#define SUPPLY5_VG_COLOR    0b0000001000011111
#define SUPPLY3_VT_COLOR    0b0000001100011111
#define SUPPLY3_VG_COLOR    0b0000001100011111
#define BATTERY_IT_COLOR    0b1111100000000000
#define BATTERY_IG_COLOR    0b1111100000000000
#define SUPPLY5_IT_COLOR    0b1111101000000000
#define SUPPLY5_IG_COLOR    0b1111101000000000
#define SUPPLY3_IT_COLOR    0b1111101100000000
#define SUPPLY3_IG_COLOR    0b1111101100000000

class LgMeasure {
public:
    char* description;
    uint8_t pin;
    uint8_t decimal;
    uint16_t color;
    float factor;
    LgMeasure(const char* _description, uint8_t _pin, uint8_t _decimal,
              uint16_t _color, float _value);
};

#endif
