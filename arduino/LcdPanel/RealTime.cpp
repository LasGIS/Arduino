#include "LcdPanel.h"
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>
#include "lcd_screen.h"

#define RT_MAX_FIELDS 6

extern char comBuffer[50];
extern DS1302 rtc;
extern LiquidCrystal_I2C lcd;

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

LcdField RTFields[RT_MAX_FIELDS + 1] {
  {0, 0,  1, 1, 7, 1, dayAsString},   // день недели
  {0, 4,  4, 1, 9999, 2015, NULL},    // год
  {0, 9,  2, 1, 12, 1, NULL},         // месяц
  {0, 12, 2, 1, 31, 1, NULL},         // день месяца
  {1, 7,  2, 0, 23, 0, NULL},         // час
  {1, 10, 2, 0, 59, 0, NULL},         // минута
  {1, 13, 2, 0, 59, 0, NULL}          // секунда
};

/** временнАя чать. */
void printOnlyTime(uint8_t row, Time* t) {
  snprintf(comBuffer, sizeof(comBuffer), "Time = %02d:%02d:%02d", t->hr, t->min, t->sec);
  lcd.setCursor(0, row);
  lcd.print(comBuffer);
}

/**
 * выводим время и дату на LCD.
 */
void printTime(LPShowModeType showMode) {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  const String day = dayAsString(t.day);

  switch (showMode) {
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
    break;
  default:
    break;
  }
}

void Time2Fields(Time tm) {
  RTFields[0].val = tm.day;
  RTFields[1].val = tm.yr;
  RTFields[2].val = tm.mon;
  RTFields[3].val = tm.date;

  RTFields[4].val = tm.hr;
  RTFields[5].val = tm.min;
  RTFields[6].val = tm.sec;
}

Time Fields2Time() {
  Time tm(2099, 1, 1, 0, 0, 0, Time::kSunday);
  tm.day = (Time::Day) RTFields[0].val;
  tm.yr = RTFields[1].val;
  tm.mon = RTFields[2].val;
  tm.date = RTFields[3].val;

  tm.hr = RTFields[4].val;
  tm.min = RTFields[5].val;
  tm.sec = RTFields[6].val;
  return tm;
}

/**
 * Редактирование времени
 */
LPModeType editTime(char key) {
  static int nField = 0;
  static int nPosit = 0;

  if (key >= '0' && key <= '9') {
    RTFields[nField].setValue(nPosit, key);
    RTFields[nField].showField(nPosit);
    key = '>';
  }

  switch(key) {
  case 1: // начальная 
    Time2Fields(rtc.time());
    lcd.clear();
    printTime(DataTime);
    lcd.cursor();
    lcd.blink();
    nField = RT_MAX_FIELDS;
    nPosit = 0;
    break;
  case '>':
    nPosit++;
    if (nPosit >= RTFields[nField].len) {
      if (nField < RT_MAX_FIELDS) {
        nField++;
        nPosit = 0;
      } else {
        nPosit--;
      }
    }
    break;
  case '<':
    nPosit--;
    if (nPosit < 0) {
      if (nField > 0) {
        nField--;
      }
      nPosit = RTFields[nField].len - 1;
    }
    break;
  case '+':
  case '-':
    RTFields[nField].setValue(nPosit, key);
    break;
  case 'p': // записываем и выходим
    // initialize real time clock. 
    rtc.writeProtect(false);
    rtc.halt(false);
    rtc.time(Fields2Time());
    lcd.noCursor();
    lcd.noBlink();
    return show;
  case 'b': // выходим без записи
    lcd.noCursor();
    lcd.noBlink();
    return show;
  }
  RTFields[nField].showField(nPosit);
  return edit;
}
