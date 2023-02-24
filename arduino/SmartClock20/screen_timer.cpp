#include "SmartClock20.h"

#define SECONDS_FROM_1970_TO_2000 946684800

ScreenTimer::ScreenTimer(): ScreenTft(AT24C_ScreenTimer) {}

void ScreenTimer::changeOrientation() {
  start();
  ScreenTft::changeOrientation();
}

void ScreenTimer::showTime(DateTime * dateTime) {
  static boolean isMusicAlarm = true;
  ScreenTft::showTime(dateTime);
  uint32_t longTime = Clock.now().unixtime();
  long deltaTime = time - longTime;
  if (deltaTime >= 0L)  {
#ifdef HAS_SERIAL
    Serial.print("deltaTime = ");
    Serial.println(deltaTime);
#endif
    DateTime dateTimer(SECONDS_FROM_1970_TO_2000 + deltaTime);
    printBigTime(&dateTimer);
  }
  if (deltaTime > -10L && deltaTime <= 0L && isMusicAlarm) {
    isMusicAlarm = false;
    printText(1, 5, 2, "Время закончилось", COLOR_GREEN);
    musicAlarm();
    isMusicAlarm = true;
  }
}

/*void ScreenTimer::showEveryTime() {}*/

void ScreenTimer::showOnce() {
  start();
  ScreenTft::showOnce();
}

/**
 * Редактирование
 */
void ScreenTimer::edit(char key) {
  switch(key) {
  case 1: // начальная
    nField = 1;
    nPosit = 0;
    load();
    break;
  case 'M': // записываем
    save();
    break;
  }
  ScreenTft::edit(key);
}

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
  // поправляем смещение от ориентации
  int8_t addx = isHorisontalOrientation() ? 2 : 1;
  for (int8_t i = 0; i <= maxFields; i++) {
    fields[i].col = i * 3 + addx;
  }
  // вычисляем время Ч
  time = Clock.now().unixtime() + startTime;
  isRedraw = true;
}
