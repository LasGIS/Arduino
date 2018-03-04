#ifndef SMARTCLOCK20_H
#define SMARTCLOCK20_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <SPI.h>
#include <Wire.h>
#include <TFT_22_ILI9225.h>
#include <IrControl.h>

//#define HAS_SERIAL
#define HAS_SERIAL_DEBUG
#define ADXL345_ENABLED

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
#define BOXV_Y0 25
#define BOXV_Y1 219
#define BOXH_X0 0
#define BOXH_X1 219
#define BOXH_Y0 25
#define BOXH_Y1 175

#define BOXCLOCKV_X0 26
#define BOXCLOCKV_X1 150
#define CLOCKV_X 30
#define BOXCLOCKV_Y0 25
#define BOXCLOCKV_Y1 53
#define CLOCKV_Y 29

#define BOXCLOCKH_X0 48
#define BOXCLOCKH_X1 172
#define CLOCKH_X 52
#define BOXCLOCKH_Y0 25
#define BOXCLOCKH_Y1 53
#define CLOCKH_Y 29

#define BOXV_CENTER_X 87
#define BOXV_CENTER_Y 109
#define BOXH_CENTER_X 109
#define BOXH_CENTER_Y 87

extern TFT_22_ILI9225 tft;
extern char comBuffer[20];
extern uint16_t X0, X1, Y0, Y1;
extern uint16_t ClockX0, ClockX1, ClockY0, ClockY1;
extern uint16_t clockX;
extern uint16_t clockY;
extern uint16_t boxCenterX;
extern uint16_t boxCenterY;

extern void printRealTime();
extern void printRealDate();
extern void drawDouble(uint16_t x, uint16_t y, double val, uint16_t color);

class GravVector {
public:
  GravVector() {
    X = 0.0;
    Y = 0.0;
    Z = 0.0;
  }
  GravVector(double _X, double _Y, double _Z) {
    X = _X;
    Y = _Y;
    Z = _Z;
  }
  double X;
  double Y;
  double Z;
};


#ifdef ADXL345_ENABLED
extern void accelBegin();
extern GravVector accelReadVector();
extern void accelUpdate(GravVector vec);
#endif

#endif // SMARTCLOCK20_H
