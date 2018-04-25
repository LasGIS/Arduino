#include "SmartClock20.h"
#include <DS3231.h>
#include "screen_timer.h"

extern ModeType mode;
extern DS3231 Clock;
extern bool Century;
extern bool h12, PM;

ScreenTimer::ScreenTimer(): ScreenTft() {
  name = (char*) "Timer ";
  maxFields = 2;
  fields = new FieldTft[maxFields + 1];
  fields[0] = {0, 12, 2, 0, 23, 0, NULL};         // час
  fields[1] = {0, 15, 2, 0, 59, 0, NULL};         // минута
  fields[2] = {0, 18, 2, 0, 59, 0, NULL};         // секунда
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
