#ifndef LCD22_h
#define LCD22_h

#include <Arduino.h>
#include "TFTv2.h"

#define screenVSize   200
#define screenHSize   288

#define screenTop     29
#define screenBottom  screenTop + screenVSize
#define screenLeft    16
#define screenRigth   screenLeft + screenHSize

#define TEMPERATURE_START 10.0
#define TEMPERATURE_DELTA 10.0
#define TEMPERATURE_MULTIPLIER 5.0

#define VOLTAGE_START 3.0
#define VOLTAGE_DELTA 0.5
#define VOLTAGE_MULTIPLIER 66.66

#define CURRENT_START 0.0
#define CURRENT_DELTA 0.01
#define CURRENT_MULTIPLIER 2000.0

//#define TIME_TYPE_6_MIN
#define TIME_TYPE_60_MIN
//#define TIME_TYPE_8_HOUR
//#define TIME_TYPE_24_HOUR

#if defined (TIME_TYPE_6_MIN)
  /* весь экран - 6 мин */
  #define TIME_MULTIPLIER 1250
  #define TIME_MAIN_MARK 60000
  #define TIME_HALF_MARK 20000
  #define TIME_FUL_SCREEN 360000
#elif defined (TIME_TYPE_60_MIN)
  /* весь экран - 1 час */
  #define TIME_MULTIPLIER 12500
  #define TIME_MAIN_MARK 600000
  #define TIME_HALF_MARK 300000
  #define TIME_FUL_SCREEN 3600000
#elif defined (TIME_TYPE_8_HOUR)
  /* весь экран - 8 часов */
  #define TIME_MULTIPLIER 100000
  #define TIME_MAIN_MARK 3600000
  #define TIME_HALF_MARK 1200000
  #define TIME_FUL_SCREEN 28800000
#elif defined (TIME_TYPE_24_HOUR)
  /* весь экран - 24 часа */
  #define TIME_MULTIPLIER 300000
  #define TIME_MAIN_MARK 10800000
  #define TIME_HALF_MARK 3600000
  #define TIME_FUL_SCREEN 86400000
#endif

#define CHARGER_VT_COLOR    COLOR(0,255,0)
#define BATTERY_VT_COLOR    COLOR(0,255,255)
#define SUPPLY5_VT_COLOR    COLOR(0,0,255)
#define SUPPLY3_VT_COLOR    COLOR(0,128,255)
#define BATTERY_IT_COLOR    COLOR(255,0,0)
#define SUPPLY5_IT_COLOR    COLOR(255,255,0)
#define SUPPLY3_IT_COLOR    COLOR(255,0,255)

#define voltColor        COLOR(32,32,255)
#define colorTime        COLOR(120,120,255)
#define markColor        COLOR(96,96,96)
#define markHourColor    COLOR(144,144,144)
#define markMinColor     COLOR(96,96,96)
#define foneColor        COLOR(24,24,24)

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
