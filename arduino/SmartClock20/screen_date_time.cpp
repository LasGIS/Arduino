#include "SmartClock20.h"
#include "screen_date_time.h"

ScreenDateTime::ScreenDateTime(): ScreenTft(AT24C_ScreenDateTime) {
  fields[6].getValue = dayOfWeekName;
}

void ScreenDateTime::showTime(DateTime* dateTime) {
  printBigTime(dateTime);
  ScreenTft::showTime(dateTime);
}

void ScreenDateTime::showEveryTime() {
#ifdef HAS_DRAW_BOBBER
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
  } else {
    uint8_t x_colon[2], fontSize;
    if (isHorisontalOrientation()) {
      fontSize = 5;
      x_colon[0] = BOXCLOCKH_X_COLON_0;
      x_colon[1] = BOXCLOCKH_X_COLON_1;
    } else {
      fontSize = 4;
      x_colon[0] = BOXCLOCKV_X_COLON_0;
      x_colon[1] = BOXCLOCKV_X_COLON_1;
    }
    for (int i = 0; i < 2; i++) {
      printTextPrec(x_colon[i], ClockY0, 0, 0, fontSize, ":  :", COLOR_TOMATO);
    }
  }
}

/**
 *
 */
void ScreenDateTime::edit(char key) {
  switch (key) {
    case 1:  // начальная
      nField = 4;
      nPosit = 0;
      load();
      break;
    case 'P':  // записываем
      save();
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

  Clock.setSecond(fields[5].val);  // Set the second
  Clock.setMinute(fields[4].val);  // Set the minute
  Clock.setHour  (fields[3].val);  // Set the hour

  Clock.setDate  (fields[0].val);  // Set the date of the month
  Clock.setMonth (fields[1].val);  // Set the month of the year
  Clock.setYear  (fields[2].val - 2000);  // Set the year (Last two digits of the year)

  Clock.setDoW(fields[6].val);  // Set the day of the week
}
