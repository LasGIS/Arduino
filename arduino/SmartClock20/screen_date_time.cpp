#include "SmartClock20.h"
#include "screen_date_time.h"

extern LPModeType mode;

ScreenDateTime::ScreenDateTime(): ScreenTft() {
  maxFields = 6;
  fields = new FieldTft[maxFields + 1];
  fields[0] = {0, 0,  1, 1, 7, 1, NULL};   // день недели
  fields[1] = {0, 4,  4, 1, 9999, 2015, NULL};    // год
  fields[2] = {0, 9,  2, 1, 12, 1, NULL};         // месяц
  fields[3] = {0, 12, 2, 1, 31, 1, NULL};         // день месяца
  fields[4] = {1, 7,  2, 0, 23, 0, NULL};         // час
  fields[5] = {1, 10, 2, 0, 59, 0, NULL};         // минута
  fields[6] = {1, 13, 2, 0, 59, 0, NULL};         // секунда
}

void ScreenDateTime::showEveryTime() {
  if (mode == show) {
  }
}

/**
 * выводим время и дату на LCD.
 */
void ScreenDateTime::showOnce() {
}

/**
 * Редактирование времени
 */
void ScreenDateTime::edit(char key) {
  ScreenTft::edit(key);
}

void ScreenDateTime::control(char key) {
  ScreenTft::control(key);
}
