#include "SmartClock20.h"
#include "screen_timer.h"

extern ModeType mode;

ScreenTimer::ScreenTimer(): ScreenTft() {
  name = (char*) "Timer ";
  maxFields = 2;
  fields = new FieldTft[maxFields + 1];
  fields[0] = {1, 7,  2, 0, 23, 0, NULL};         // час
  fields[1] = {1, 10, 2, 0, 59, 0, NULL};         // минута
  fields[2] = {1, 13, 2, 0, 59, 0, NULL};         // секунда
}

void ScreenTimer::showTime() {
  ScreenTft::showTime();
}

/*void ScreenTimer::showEveryTime() {
  if (mode == show) {
  }
}*/

/**
 *
 */
/*void ScreenTimer::showOnce() {
  ScreenTft::showOnce();
}*/

/**
 * Редактирование
 */
void ScreenTimer::edit(char key) {
  ScreenTft::edit(key);
}

/*void ScreenTimer::control(char key) {
  ScreenTft::control(key);
}*/
