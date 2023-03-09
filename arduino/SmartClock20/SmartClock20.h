#ifndef SMARTCLOCK20_H
#define SMARTCLOCK20_H

#include <Arduino.h>
//#include <avr/pgmspace.h>
#include <SerialBlock.h>
#include <LoadClass.h>
#include <SPI.h>
#include <DS3231.h>
#include <Wire.h>
#include <TFT_LG_ILI9225.h>
#include <IrControl.h>
#include "Memory.h"
#include "screen_tft.h"
#include "screen_date_time.h"
#include "screen_timer.h"
#include "screen_dump.h"

// включаем режим отладки
//#define HAS_SERIAL

#define DEVICE 0x57

#define ADXL345_ENABLED

// пин для жужалки
#define BUZZER_PIN 7

//0.577 or 0.707
#define GRAVI_FACTOR 0.8
#define BOXV_X0 0
#define BOXV_X1 175
#define BOXV_Y0 17
#define BOXV_Y1 219
#define BOXH_X0 0
#define BOXH_X1 219
#define BOXH_Y0 17
#define BOXH_Y1 175

#define BOXCLOCKV_X0 13
#define BOXCLOCKV_X1 163
#define BOXCLOCKV_Y0 19
#define BOXCLOCKV_Y1 49

#define BOXCLOCKH_X0 31
#define BOXCLOCKH_X1 181
#define BOXCLOCKH_Y0 19
#define BOXCLOCKH_Y1 49

#define BOXV_CENTER_X 87
#define BOXV_CENTER_Y 109
#define BOXH_CENTER_X 109
#define BOXH_CENTER_Y 87

extern TFT_LG_ILI9225 tft;
extern DS3231 Clock;
extern LoadClass loadClass;
extern char comBuffer[20];
extern ModeType mode;
extern uint8_t isRedraw;
extern uint8_t keySoundVolume;

extern uint16_t X0, X1, Y0, Y1;
extern uint16_t ClockX0, ClockX1, ClockY0, ClockY1;
extern uint16_t boxCenterX;
extern uint16_t boxCenterY;

extern void musicAlarm();
extern void buzzerOut (uint16_t hertz, uint32_t del, uint8_t soundVolume);
extern void printText (uint8_t col, uint8_t row, uint8_t fontSize, const char * text, uint16_t color);
extern void setCursor (uint8_t col, uint8_t row, uint8_t fontSize);
extern void drawDouble(uint8_t col, uint8_t row, uint8_t fontSize, double val, uint16_t color);
extern void drawHex   (uint8_t col, uint8_t row, uint8_t fontSize, uint16_t val, uint8_t size, uint16_t color);

extern const char* dayOfWeekName(const int dayOfWeek);
extern void printShortTime(DateTime* dateTime);
extern void printShortDate(DateTime* dateTime);
extern void printBigTime(DateTime* dateTime);
extern void printDayOfWeek(DateTime* dateTime);

extern bool isHorisontalOrientation();

class GravVector {
public:
  GravVector();
  GravVector(double _X, double _Y, double _Z);
  void set(double _X, double _Y, double _Z);
  double X;
  double Y;
  double Z;
};
extern GravVector gravVector;

#ifdef ADXL345_ENABLED
extern void accelBegin();
extern GravVector accelReadVector();
extern void accelUpdate();
#endif

#endif // SMARTCLOCK20_H
