#include "SmartClock20.h"

DS3231 Clock;
char * bufTime = (char*) "xx:xx:xx";
char * bufDate = (char*) "xx.xx.20xx";

/**
 * переводим целое в строку (две цифры)
 * @param val целое значение
 * @param str строка
 * @param start начало вывода в строке
 */
void toTwoChar(uint8_t val, char * str, uint8_t start) {
  uint8_t fst = (val % 100)/ 10;
  uint8_t lst = val % 10;
  str[start] = 0x30 + fst;
  str[start + 1] = 0x30 + lst;
}

const char* dayOfWeekName(const int dayOfWeek) {
  LoadClass lc(DEVICE, 0);
  if (isHorisontalOrientation()) {
    lc.toArrayItem(AT24C_DayOfWeekHorizontal, (dayOfWeek - 1));
  } else {
    lc.toArrayItem(AT24C_DayOfWeekVertical, (dayOfWeek - 1));
  }
  char* str = lc.readString();
  memcpy(comBuffer, str, strlen(str) + 1);
  return comBuffer;
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
//  tft.setBackgroundColor(COLOR_GRAY);
  if (isRedraw || minLast != min || hourLast != hour) {
    toTwoChar(hour, bufTime, 0);
    toTwoChar(min, bufTime, 3);
    toTwoChar(sec, bufTime, 6);
    printText(clockX, 1, 3, bufTime, COLOR_TOMATO);
    minLast = min;
    hourLast = hour;
  } else {
    toTwoChar(sec, bufTime, 6);
#ifdef HAS_SERIAL
    Serial.println(bufTime);
#endif
    printText(clockX + 6, 1, 3, bufTime + 6, COLOR_TOMATO);
  }
}

/**
 * выводим короткое время (в заголовок)
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
    printText(11, 0, 1, bufTime, COLOR_WHITE);
    minLast = min;
    hourLast = hour;
  } else {
    toTwoChar(sec, bufTime, 6);
    printText(17, 0, 1, bufTime + 6, COLOR_WHITE);
  }
}

/**
 * выводим короткую дату (в заголовок)
 */
void printShortDate(DateTime * dateTime) {
  static uint8_t dayLast   = 0xff;
  static uint8_t monthLast = 0xff;
  static uint8_t yearLast  = 0xff;
  uint8_t day    = dateTime->day();
  uint8_t month  = dateTime->month();
  uint8_t year   = dateTime->year() - 2000;
  if (isRedraw || dayLast != day || monthLast != month || yearLast != year) {
    toTwoChar(day, bufDate, 0);
    toTwoChar(month, bufDate, 3);
    toTwoChar(year, bufDate, 8);
#ifdef HAS_SERIAL
    Serial.println(buf);
#endif
    printText(0, 0, 1, bufDate, COLOR_WHITE);
    dayLast   = day;
    monthLast = month;
    yearLast  = year;
  }
}

/**
 * выводим день недели в заголовок.
 */
void printDayOfWeek(DateTime * dateTime) {
  static uint8_t dayOfWeekLast = 0xff;
  uint8_t dayOfWeek = dateTime->dayOfWeek();
  if (isRedraw || dayOfWeekLast != dayOfWeek) {
    char * str = dayOfWeekName(dayOfWeek);
    printText(20, 0, 1, str, COLOR_BROWN);
    dayOfWeekLast = dayOfWeek;
  }
}
