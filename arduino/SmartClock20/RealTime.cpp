#include "SmartClock20.h"

DS3231 Clock;
bool Century = false;
bool h12, PM;
char * bufTime = (char*) "xx:xx:xx";

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
 * выводим короткое время (в заголовок).
 */
void printShortTime(DateTime * dateTime) {
  static uint8_t minLast  = 0xff;
  static uint8_t hourLast = 0xff;
  uint8_t hour = dateTime->hour();
  uint8_t min = dateTime->minute();
  uint8_t sec = dateTime->second();
  if (isRedraw || minLast != min || hourLast != hour) {
    toTwoChar(hour, bufTime, 0);
    toTwoChar(min, bufTime, 3);
    toTwoChar(sec, bufTime, 6);
    printText(12, 0, bufTime, COLOR_WHITE);
    minLast = min;
    hourLast = hour;
  } else {
    toTwoChar(sec, bufTime, 6);
    printText(18, 0, bufTime + 6, COLOR_WHITE);
  }
}

/**
 * выводим большое время (посередине).
 */
void printBigTime(DateTime * dateTime) {
  static uint8_t minLast  = 0xff;
  static uint8_t hourLast = 0xff;
  uint8_t clockX = isHorisontalOrientation() ? 2 : 1;
  uint8_t hour = dateTime->hour();
  uint8_t min = dateTime->minute();
  uint8_t sec = dateTime->second();
  tft.setFontSize(3);
//  tft.setBackgroundColor(COLOR_GRAY);
  if (isRedraw || minLast != min || hourLast != hour) {
    toTwoChar(hour, bufTime, 0);
    toTwoChar(min, bufTime, 3);
    toTwoChar(sec, bufTime, 6);
    printText(clockX, 1, bufTime, COLOR_TOMATO);
    minLast = min;
    hourLast = hour;
  } else {
    toTwoChar(sec, bufTime, 6);
    printText(clockX + 6, 1, bufTime + 6, COLOR_TOMATO);
  }
  tft.setFontSize(1);
}

/**
 * выводим реальную дату.
 */
void printRealDate(DateTime * dateTime) {
  static char * buf = (char*) "xx.xx.20xx";
  static uint8_t dayLast   = 0xff;
  static uint8_t monthLast = 0xff;
  static uint8_t yearLast  = 0xff;
  uint8_t day    = dateTime->day();
  uint8_t month  = dateTime->month();
  uint8_t year   = dateTime->year() - 2000;
  if (isRedraw || dayLast != day || monthLast != month || yearLast != year) {
    toTwoChar(day, buf, 0);
    toTwoChar(month, buf, 3);
    toTwoChar(year, buf, 8);
#ifdef HAS_SERIAL
    Serial.println(buf);
#endif
    printText(0, 0, buf, COLOR_WHITE);
    dayLast   = day;
    monthLast = month;
    yearLast  = year;
  }
}

