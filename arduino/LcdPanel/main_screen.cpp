#include "LcdPanel.h"
#include "LcdPanel.h"
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>
#include "main_screen.h"

#define RT_MAX_FIELDS 6

// время в миллисекундах
unsigned long milliSec;

extern char comBuffer[50];
extern DS1302 rtc;
extern LiquidCrystal_I2C lcd;
//extern LPShowModeType showMode;

String dayAsString(const uint16_t val) {
  Time::Day day = (Time::Day) val;
  switch (day) {
    case Time::kSunday: return "Sun";
    case Time::kMonday: return "Mon";
    case Time::kTuesday: return "Tue";
    case Time::kWednesday: return "Wed";
    case Time::kThursday: return "Thu";
    case Time::kFriday: return "Fri";
    case Time::kSaturday: return "Sat";
  }
  return "(und)";
}

MainScreen::MainScreen(): LcdScreen() {
  maxFields = RT_MAX_FIELDS;
  fields = new LcdField[maxFields + 1];
  fields[0] = {0, 0,  1, 1, 7, 1, dayAsString};   // день недели
  fields[1] = {0, 4,  4, 1, 9999, 2015, NULL};    // год
  fields[2] = {0, 9,  2, 1, 12, 1, NULL};         // месяц
  fields[3] = {0, 12, 2, 1, 31, 1, NULL};         // день месяца
  fields[4] = {1, 7,  2, 0, 23, 0, NULL};         // час
  fields[5] = {1, 10, 2, 0, 59, 0, NULL};         // минута
  fields[6] = {1, 13, 2, 0, 59, 0, NULL};         // секунда

  showMode = (LPShowModeType) EEPROM.read(SHOW_MODE_ADR);
  if (showMode > Battery) {
    showMode = BigTime;
    EEPROM.update(SHOW_MODE_ADR, showMode);
  }
}

void MainScreen::showEveryTime(int count) {
  if (mode == show) {
    /* Показываем время */
    unsigned long msec = millis();
    if ((msec - milliSec) / 100 > 0) {
      milliSec = msec;
      showOnce();
    }
    if (count % 20 == 1) {
      if (showMode == TimeHum || showMode == Humidity) {
        temperatureHumidity();
      } else if (showMode == Battery) {
        batteryCapasity();
      }
    }
  }
}

/**
 * выводим время и дату на LCD.
 */
void MainScreen::showOnce() {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  const String day = dayAsString(t.day);

  switch (mode) {
  case BigTime:
    viewCustomDigit(0, t.hr / 10);
    viewCustomDigit(4, t.hr % 10);
    viewCustomDigit(9, t.min / 10);
    viewCustomDigit(13, t.min % 10);
    lcd.setCursor(7, 1);
    snprintf(comBuffer, sizeof(comBuffer), "%02d", t.sec);
    lcd.print(comBuffer);
    break;
  case DataTime:
    snprintf(comBuffer, sizeof(comBuffer), "%s %04d-%02d-%02d ", day.c_str(), t.yr, t.mon, t.date);
    lcd.setCursor(0, 0);
    lcd.print(comBuffer);
    printOnlyTime(1, &t);
    break;
  case TimeHum:
    printOnlyTime(0, &t);
    break;
  case Battery:
  default:
    break;
  }
}

void MainScreen::Time2Fields(Time tm) {
  fields[0].val = tm.day;
  fields[1].val = tm.yr;
  fields[2].val = tm.mon;
  fields[3].val = tm.date;

  fields[4].val = tm.hr;
  fields[5].val = tm.min;
  fields[6].val = tm.sec;
}

Time MainScreen::Fields2Time() {
  Time tm(2099, 1, 1, 0, 0, 0, Time::kSunday);
  tm.day = (Time::Day) fields[0].val;
  tm.yr = fields[1].val;
  tm.mon = fields[2].val;
  tm.date = fields[3].val;

  tm.hr = fields[4].val;
  tm.min = fields[5].val;
  tm.sec = fields[6].val;
  return tm;
}

/**
 * Редактирование времени
 */
void MainScreen::edit(char key) {
  switch(key) {
  case 1: // начальная
    Time2Fields(rtc.time());
    break;
  case 'p': // записываем и выходим
    // initialize real time clock.
    rtc.writeProtect(false);
    rtc.halt(false);
    rtc.time(Fields2Time());
  case 'b': // выходим без записи
    break;
  }
  LcdScreen::edit(key);
}

LPModeType MainScreen::control(char key) {
  switch (key) {
  case '+':
    lcd.clear();
    showMode = showMode > BigTime ? (LPShowModeType) (showMode - 1) : Battery;
    EEPROM.update(SHOW_MODE_ADR, showMode);
    count = 0;
    break;
  case '-':
    lcd.clear();
    showMode = showMode < Battery ? (LPShowModeType) (showMode + 1) : BigTime;
    EEPROM.update(SHOW_MODE_ADR, showMode);
    count = 0;
    break;
  case 'p':
    if (showMode == DataTime) {
      mode = realTimeScreen.edit(1);
    }
    break;
  }
}
