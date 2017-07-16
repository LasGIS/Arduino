#ifndef LCD22_h
#define LCD22_h

#include <Arduino.h>
#include "TFTv2.h"

#define TEMPERATURE_START 10.0
#define TEMPERATURE_DELTA 10.0
#define TEMPERATURE_MULTIPLIER 5.0

#define VOLTAGE_START 3.0
#define VOLTAGE_DELTA 0.5
#define VOLTAGE_MULTIPLIER 66.66

#define CURRENT_START 0.0
#define CURRENT_DELTA 0.01
#define CURRENT_MULTIPLIER 2000.0

/* весь экран - 1 час */
#define TIME_MULTIPLIER 12500
/* весь экран - 8 часов */
//#define TIME_MULTIPLIER 100000
/* весь экран - 24 часа */
//#define TIME_MULTIPLIER 300000

#define CHARGER_VT_COLOR    COLOR(0,255,0)
#define BATTERY_VT_COLOR    COLOR(0,255,255)
#define SUPPLY5_VT_COLOR    COLOR(0,0,255)
#define SUPPLY3_VT_COLOR    COLOR(0,128,255)
#define BATTERY_IT_COLOR    COLOR(255,0,0)
#define SUPPLY5_IT_COLOR    COLOR(255,255,0)
#define SUPPLY3_IT_COLOR    COLOR(255,0,255)

#define voltColor        COLOR(32,32,255)
#define colorTime        COLOR(120,120,120)
#define markColor        COLOR(96,96,96)
#define markHourColor    COLOR(144,144,144)
#define markMinColor     COLOR(96,96,96)
#define foneColor        COLOR(24,24,24)

#define screenVSize   200
#define screenHSize   288

#define screenTop     29
#define screenBottom  screenTop + screenVSize
#define screenLeft    0
#define screenRigth   screenLeft + screenHSize

class LgMeasure {
public:
    char* description;
    uint8_t pin;
    uint8_t decimal;
    uint16_t color;
    float factor;
    uint8_t typeOut;
    LgMeasure(const char* _description, uint8_t _pin, uint8_t _decimal,
              uint16_t _color, float _value, uint8_t typeOut);
};

#endif
