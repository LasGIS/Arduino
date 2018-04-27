#include "SmartClock20.h"

ScreenTimer::ScreenTimer(): ScreenTft() {
  name = (char*) "Timer ";
  maxFields = 2;
  fields = new FieldTft[maxFields + 1];
  fields[0] = {0, 12, 2, 0, 23, 0, NULL};         // час
  fields[1] = {0, 15, 2, 0, 59, 0, NULL};         // минута
  fields[2] = {0, 18, 2, 0, 59, 0, NULL};         // секунда
}

void ScreenTimer::showTime(DateTime * dateTime) {
  ScreenTft::showTime(dateTime);
}

void ScreenTimer::showEveryTime() {
}

void ScreenTimer::showOnce() {
  ScreenTft::showOnce();
}

/**
 * Редактирование
 */
void ScreenTimer::edit(char key) {
  switch(key) {
  case 1: // начальная
    nField = 0;
    nPosit = 0;
    load();
    break;
  case 'M': // записываем
    save();
    break;
  }
  ScreenTft::edit(key);
}

/*void ScreenTimer::control(char key) {
  ScreenTft::control(key);
}*/

void ScreenTimer::load() {
  fields[0].val = Clock.getHour(h12, PM);
  fields[1].val = Clock.getMinute();
  fields[2].val = Clock.getSecond();
}

void ScreenTimer::save() {

}
