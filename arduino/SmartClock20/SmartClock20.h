#ifndef SMARTCLOCK20_H
#define SMARTCLOCK20_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <SPI.h>
#include <Wire.h>
#include <TFT_22_ILI9225.h>
//#include <DS3231.h>
//#include <ADXL345.h>

//#define ADXL345_ENABLED

#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 3   // 0 if wired to +5V directly

#define BOX_X0 0
#define BOX_X1 175
#define BOX_Y0 44
#define BOX_Y1 219
#define BOX_CENTER_X 87
#define BOX_CENTER_Y 131

extern TFT_22_ILI9225 tft;
extern char comBuffer[20];
extern void printRealTime();
extern void printRealDate();
extern void drawFloat(uint16_t x, uint16_t y, double val, uint16_t color);

#ifdef ADXL345_ENABLED
extern void accelBegin();
extern void accelUpdate();
#endif

#endif // SMARTCLOCK20_H
