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


/**
 * выводим реальное время.
 */
void printRealTime() {
  int sec  = Clock.getSecond();
  int min  = Clock.getMinute();
  int hour = Clock.getHour(h12, PM);
  snprintf(comBuffer, sizeof(comBuffer), "%02d:%02d:%02d", hour, min, sec);
#ifdef HAS_SERIAL
  Serial.println(comBuffer);
#endif
  tft.setFontSize(3);
//  tft.setBackgroundColor(COLOR_BLUEVIOLET);
  tft.drawText(clockX, clockY, comBuffer, COLOR_TOMATO);
//  tft.setBackgroundColor(COLOR_BLACK);
  tft.setFontSize(1);
  tft.drawText(80, 0, comBuffer, COLOR_WHITE);
}

/**
 * выводим реальную дату.
 */
void printRealDate() {
  int date   = Clock.getDate();
  int month  = Clock.getMonth(Century);
  int year   = Clock.getYear();
  snprintf(comBuffer, sizeof(comBuffer), "%02d.%02d.20%02d ", date, month, year);
#ifdef HAS_SERIAL
  Serial.println(comBuffer);
#endif
  tft.drawText(0, 0, comBuffer, COLOR_WHITE);
}

