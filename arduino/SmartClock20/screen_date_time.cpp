#include "SmartClock20.h"
#include "screen_date_time.h"

ScreenDateTime::ScreenDateTime(): ScreenTft() {
  name = (char*) "Время ";
  maxFields = 6;
  fields = new FieldTft[maxFields + 1];

  fields[0] = {4, 3, 2, 2, 1, 31, 1, NULL};         // день месяца
  fields[1] = {4, 6, 2, 2, 1, 12, 1, NULL};         // месяц
  fields[2] = {4, 9, 2, 4, 1, 9999, 2015, NULL};    // год

  fields[3] = {5, 3, 2, 2, 0, 23, 0, NULL};         // час
  fields[4] = {5, 6, 2, 2, 0, 59, 0, NULL};         // минута
  fields[5] = {5, 9, 2, 2, 0, 59, 0, NULL};         // секунда

  fields[6] = {6, 3, 2, 1, 1,  7, 1, NULL};         // день недели (Понедельник...)
}

void ScreenDateTime::showTime(DateTime * dateTime) {
  printBigTime(dateTime);
  ScreenTft::showTime(dateTime);
}

void ScreenDateTime::showEveryTime() {
#ifdef ADXL345_ENABLED
    accelUpdate();
#endif
}

/**
 *
 */
void ScreenDateTime::showOnce() {
  ScreenTft::showOnce();
  if (mode == ModeType::edit) {
    printText(5, 4, 2, ".  .", COLOR_TOMATO);
    printText(5, 5, 2, ":  :", COLOR_TOMATO);
  }
}

/**
 *
 */
void ScreenDateTime::edit(char key) {
  switch(key) {
  case 1: // начальная
    nField = 4;
    nPosit = 0;
    load();
    break;
  case 'M': // записываем
    save();
  case 'r': // без записи
    break;
  }
  ScreenTft::edit(key);
}

/*void ScreenDateTime::control(char key) {
  ScreenTft::control(key);
}*/

void ScreenDateTime::load() {
  bool century, h12, PM;
  fields[0].val = Clock.getDate();
  fields[1].val = Clock.getMonth(century);
  fields[2].val = Clock.getYear() + 2000;

  fields[3].val = Clock.getHour(h12, PM);
  fields[4].val = Clock.getMinute();
  fields[5].val = Clock.getSecond();

  fields[6].val = Clock.getDoW();
}

void ScreenDateTime::save() {

  Clock.setClockMode(false); // 24 часа mode

  Clock.setSecond(fields[5].val);  //Set the second
  Clock.setMinute(fields[4].val);  //Set the minute
  Clock.setHour  (fields[3].val);  //Set the hour

  Clock.setDate  (fields[0].val);  //Set the date of the month
  Clock.setMonth (fields[1].val);  //Set the month of the year
  Clock.setYear  (fields[2].val - 2000);  //Set the year (Last two digits of the year)

  Clock.setDoW   (fields[6].val);  //Set the day of the week
}

