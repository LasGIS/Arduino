#include "SmartClock20.h"

#define SECONDS_FROM_1970_TO_2000 946684800

ScreenTimer::ScreenTimer(): ScreenTft() {
  name = (char*) "Timer ";
  maxFields = 2;
  fields = new FieldTft[maxFields + 1];
  fields[0] = {1, 2, 2, 0, 23, 0, NULL};         // час
  fields[1] = {1, 5, 2, 0, 59, 0, NULL};         // минута
  fields[2] = {1, 8, 2, 0, 59, 0, NULL};         // секунда
}

void ScreenTimer::changeOrientation() {
  start();
  ScreenTft::changeOrientation();
}

void ScreenTimer::showTime(DateTime * dateTime) {
  ScreenTft::showTime(dateTime);
  uint32_t longTime = RTClib().now().unixtime();
  long deltaTime = time - longTime;
  if (deltaTime >= 0L)  {
#ifdef HAS_SERIAL
    Serial.print("deltaTime = ");
    Serial.println(deltaTime);
#endif
    DateTime dateTimer(SECONDS_FROM_1970_TO_2000 + deltaTime);
    printBigTime(&dateTimer);
  }
  if (deltaTime > -10L && deltaTime <= 0L) {
    tft.setFontSize(2);
    printText(1, 5, "¬рем€ закончилось", COLOR_GREEN);
    tft.setFontSize(1);
    musicAlarm();
  }
}

void ScreenTimer::showEveryTime() {
}

void ScreenTimer::showOnce() {
  start();
  ScreenTft::showOnce();
}

/**
 * –едактирование
 */
void ScreenTimer::edit(char key) {
  switch(key) {
  case 1: // начальна€
    nField = 0;
    nPosit = 0;
    load();
    tft.setFontSize(3);
    showAllFields();
    break;
  case 'M': // записываем
    save();
  case 'r': // без записи
    tft.setFontSize(1);
    start();
    break;
  }
  ScreenTft::edit(key);
}

/*void ScreenTimer::control(char key) {
  ScreenTft::control(key);
}*/

void ScreenTimer::load() {
  uint32_t _startTime = startTime;
  fields[2].val = _startTime % 60;
  _startTime /= 60;
  fields[1].val = _startTime % 60;
  _startTime /= 60;
  fields[0].val = _startTime % 24;
}

void ScreenTimer::save() {
  startTime = (fields[0].val * 60 + fields[1].val) * 60 + fields[2].val;
}

void ScreenTimer::start() {
  // поправл€ем смещение от ориентации
  int8_t addx = isHorisontalOrientation() ? 2 : 1;
  for (int8_t i = 0; i <= maxFields; i++) {
    fields[i].col = i * 3 + addx;
  }
  // вычисл€ем врем€ „
  time = RTClib().now().unixtime() + startTime;
  isRedraw = true;
}
