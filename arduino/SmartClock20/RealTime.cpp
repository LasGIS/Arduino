#include "SmartClock20.h"
#include <DS3231.h>
//#include <Wire.h>

DS3231 Clock;
bool Century = false;
bool h12, PM;

/*
void ReadDS3231() {
  int sec  = Clock.getSecond();
  int min  = Clock.getMinute();
  int hour   = Clock.getHour(h12, PM);
  int date   = Clock.getDate();
  int month  = Clock.getMonth(Century);
  int year   = Clock.getYear();

  int temperature = Clock.getTemperature();

#ifdef HAS_SERIAL
  Serial.print("20");
  Serial.print(year, DEC);
  Serial.print('-');
  Serial.print(month, DEC);
  Serial.print('-');
  Serial.print(date, DEC);
  Serial.print(' ');
  Serial.print(hour, DEC);
  Serial.print(':');
  Serial.print(min, DEC);
  Serial.print(':');
  Serial.print(sec, DEC);
  Serial.print("; ");
  Serial.print("Temperature=");
  Serial.print(temperature);
  Serial.print('\n');
#endif
}
*/
void toTwoChar(uint8_t val, char * str, uint8_t start) {
  uint8_t fst = (val % 100)/ 10;
  uint8_t lst = val % 10;
  str[start] = 0x30 + fst;
  str[start + 1] = 0x30 + lst;
}

/**
 * выводим реальное время.
 */
void printRealTime() {
  static char * buf = (char*) "xx:xx:xx";
  static uint8_t minLast  = 0xff;
  static uint8_t hourLast = 0xff;
  uint8_t sec  = Clock.getSecond();
  uint8_t min  = Clock.getMinute();
  uint8_t hour = Clock.getHour(h12, PM);
  if (isChangeOrientation || minLast != min || hourLast != hour) {
    toTwoChar(hour, buf, 0);
    toTwoChar(min, buf, 3);
    toTwoChar(sec, buf, 6);

#ifdef HAS_SERIAL
    Serial.println(buf);
#endif
    tft.setFontSize(3);
    tft.drawText(clockX, clockY, buf, COLOR_TOMATO);
    tft.setFontSize(1);
    printText(12, 0, buf, COLOR_WHITE);
    minLast = min;
    hourLast = hour;
  } else {
    toTwoChar(sec, buf, 6);
    tft.setFontSize(3);
    tft.drawText(clockX + 108, clockY, buf + 6, COLOR_TOMATO);
    tft.setFontSize(1);
    printText(18, 0, buf + 6, COLOR_WHITE);
  }
}

/**
 * выводим реальную дату.
 */
void printRealDate() {
  static char * buf = (char*) "xx.xx.20xx";
  static uint8_t dateLast   = 0xff;
  static uint8_t monthLast  = 0xff;
  static uint8_t yearLast   = 0xff;
  uint8_t date   = Clock.getDate();
  uint8_t month  = Clock.getMonth(Century);
  uint8_t year   = Clock.getYear();
  if (isChangeOrientation || dateLast != date || monthLast != month || yearLast != year) {
    toTwoChar(date, buf, 0);
    toTwoChar(month, buf, 3);
    toTwoChar(year, buf, 8);
#ifdef HAS_SERIAL
    Serial.println(buf);
#endif
    printText(0, 0, buf, COLOR_WHITE);
    dateLast  = date;
    monthLast = month;
    yearLast  = year;
  }
}

