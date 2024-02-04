#ifndef SMART_CLOCK_WEB_32_H
#define SMART_CLOCK_WEB_32_H

#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <DS3231.h>
#include <TFT_eSPI.h>

extern WiFiServer server;
extern TFT_eSPI tft;
extern DS3231 Clock;

#define START_X 5
#define START_W 310
#define START_Y 3
#define START_H 234

#define CLOCK_X 5
#define CLOCK_W 310
#define CLOCK_Y 70
#define CLOCK_H 100

#define VOLT_X 40
#define VOLT_W 240
#define VOLT_Y 180
#define VOLT_H 50

/** общие переменные */
extern String bufTime;
extern String bufDate;

/** общие функции */
extern void toTwoChar(int val, String& str, unsigned int start);

#endif // SMART_CLOCK_WEB_32_H
