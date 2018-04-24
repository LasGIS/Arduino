#include "SmartClock20.h"
#include "screen_date_time.h"

extern ModeType mode;

ScreenDateTime::ScreenDateTime(): ScreenTft() {
  name = (char*) "Main  ";
  maxFields = 6;
  fields = new FieldTft[maxFields + 1];
  fields[0] = {0, 0,  1, 1, 7, 1, NULL};   // день недели
  fields[1] = {0, 4,  4, 1, 9999, 2015, NULL};    // год
  fields[2] = {0, 9,  2, 1, 12, 1, NULL};         // мес€ц
  fields[3] = {0, 12, 2, 1, 31, 1, NULL};         // день мес€ца
  fields[4] = {1, 7,  2, 0, 23, 0, NULL};         // час
  fields[5] = {1, 10, 2, 0, 59, 0, NULL};         // минута
  fields[6] = {1, 13, 2, 0, 59, 0, NULL};         // секунда
}

void ScreenDateTime::showTime() {
  ScreenTft::showTime();
}

void ScreenDateTime::showEveryTime() {
  if (mode == show) {
#ifdef ADXL345_ENABLED
    accelUpdate();
#endif
  }
}

/**
 *
 */
/*void ScreenDateTime::showOnce() {
  ScreenTft::showOnce();
}*/

/**
 *
 */
void ScreenDateTime::edit(char key) {
  ScreenTft::edit(key);
}

void ScreenDateTime::control(char key) {
  ScreenTft::control(key);
}
