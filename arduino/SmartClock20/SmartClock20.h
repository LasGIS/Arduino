#ifndef SMARTCLOCK20_H
#define SMARTCLOCK20_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <SPI.h>
#include <Wire.h>
//#include <EEPROM.h>
#include <TFT_LG_ILI9225.h>
#include <IrControl.h>
#include "screen_tft.h"
#include "screen_date_time.h"
#include "screen_timer.h"


//#define HAS_SERIAL
//#define HAS_SERIAL_DEBUG

#define ADXL345_ENABLED

// пин для жужалки
#define BUZZER_PIN 7

#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 3   // 0 if wired to +5V directly

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
#define CLOCKV_X 1
//#define CLOCKV_X 17
#define BOXCLOCKV_Y0 19
#define BOXCLOCKV_Y1 49
#define CLOCKV_Y 1
//#define CLOCKV_Y 30

#define BOXCLOCKH_X0 31
#define BOXCLOCKH_X1 181
#define CLOCKH_X 2
//#define CLOCKH_X 39
#define BOXCLOCKH_Y0 19
#define BOXCLOCKH_Y1 49
#define CLOCKH_Y 1
//#define CLOCKH_Y 30

#define BOXV_CENTER_X 87
#define BOXV_CENTER_Y 109
#define BOXH_CENTER_X 109
#define BOXH_CENTER_Y 87

/* количество экранов */
#define NUMBER_OF_SCREENS 2
extern TFT_LG_ILI9225 tft;
extern char comBuffer[20];
extern uint8_t isChangeOrientation;
extern uint16_t X0, X1, Y0, Y1;
extern uint16_t ClockX0, ClockX1, ClockY0, ClockY1;
extern uint16_t clockX;
extern uint16_t clockY;
extern uint16_t boxCenterX;
extern uint16_t boxCenterY;
extern uint8_t keySoundVolume;

extern void musicAlarm();
extern void buzzerOut(uint16_t hertz, uint32_t del, uint8_t soundVolume);
extern void printText(uint16_t col, uint16_t row, const char * text, uint16_t color);
extern void setCursor(uint16_t col, uint16_t row);
extern void printRealTime();
extern void printRealDate();
extern void drawDouble(uint16_t x, uint16_t y, double val, uint16_t color);

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
