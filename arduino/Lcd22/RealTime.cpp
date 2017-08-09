#include <DS3231.h>
#include "Lcd22.h"

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
 * @brief drawFillString
 * @param string
 * @param poX
 * @param poY
 * @param size размер шрифта
 * @param bgColor цвет фона
 * @param color цвет символов
 */
void drawFillString(
  char *string,
  INT16U poX, INT16U poY, INT16U fontSize,
  INT16U bgColor, INT16U color
) {
  Tft.fillRectangle(poX, poY,
    strlen(string) * FONT_SPACE * fontSize, FONT_Y * fontSize, bgColor);
  Tft.drawString(string, poX, poY, fontSize, color);
}

/**
 * выводим время и дату в формате.
 */
void printTime(long time) {
  unsigned long milTime = time / 1000;
  int sec = milTime % 60;
  int min = (milTime / 60) % 60;
  int hour = milTime / 3600;
  snprintf(comBuffer, sizeof(comBuffer), "%02d:%02d:%02d", hour, min, sec);
  drawFillString(comBuffer, 260, 0, FONT_SIZE, BLACK, colorTime);
}

void beforePrintBigTime() {
  int x = 259, y = CHAR_HEIGHT + 2;
  Tft.drawChar(':', x, y, 2, WHITE);
  x += 30;// y += 6;
  Tft.drawChar(':', x, y, 2, WHITE);
}

/**
 * выводим время Крупно.
 */
void printBigTime() {
  int x = 238, y = CHAR_HEIGHT + 2;
  int sec  = Clock.getSecond();
  int min  = Clock.getMinute();
  int hour   = Clock.getHour(h12, PM);
  snprintf(comBuffer, sizeof(comBuffer), "%02d", hour);
  drawFillString(comBuffer, x, y, 2, BLACK, colorRealTime);
  x += 30;
  snprintf(comBuffer, sizeof(comBuffer), "%02d", min);
  drawFillString(comBuffer, x, y, 2, BLACK, colorRealTime);
  x += 30;// y += 6;
  snprintf(comBuffer, sizeof(comBuffer), "%02d", sec);
  drawFillString(comBuffer, x, y, 2, BLACK, colorRealTime);
}

/**
 * выводим реальное время.
 */
void printRealTime() {
  int sec  = Clock.getSecond();
  int min  = Clock.getMinute();
  int hour   = Clock.getHour(h12, PM);
  snprintf(comBuffer, sizeof(comBuffer), "%02d:%02d:%02d", hour, min, sec);
  drawFillString(comBuffer, 240, CHAR_HEIGHT, FONT_SIZE, BLACK, colorRealTime);
}

/**
 * выводим реальную дату.
 */
void printRealDate() {
  int date   = Clock.getDate();
  int month  = Clock.getMonth(Century);
  int year   = Clock.getYear();
  snprintf(comBuffer, sizeof(comBuffer), "%02d.%02d.20%02d", date, month, year);
  drawFillString(comBuffer, 240, CHAR_HEIGHT * 2, FONT_SIZE, BLACK, colorRealTime);
}

