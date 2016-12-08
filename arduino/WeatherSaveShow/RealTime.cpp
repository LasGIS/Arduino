#include <Arduino.h>
#include "WeatherSaveShow.h"

#define RT_MAX_FIELDS 6

extern TFT screen;
extern DS1302 rtc;
extern char buffer[20];

void fillPlace(int x, int y, int len, color col);
void Time2Fields(Time tm);
Time Fields2Time();
void showField(int nField, int nPosit);
void setValue(int nField, int nPosit, char key);

RTField RTFields[RT_MAX_FIELDS + 1] {
  {0, 0, 3, 7, 1},
  {0, 4, 4, 9999, 2015},
  {0, 9, 2, 12, 1},
  {0, 12, 2, 31, 1},
  {0, 18, 2, 23, 0},
  {0, 21, 2, 59, 0},
  {0, 24, 2, 59, 0}
};

/**
 * "Вск", "Пон", "Втр", "Срд", "Чет", "Пят", "Суб"
 */
char * dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: return (char *) "Вск";
    case Time::kMonday: return (char *) "Пон";
    case Time::kTuesday: return (char *) "Втр";
    case Time::kWednesday: return (char *) "Срд";
    case Time::kThursday: return (char *) "Чет";
    case Time::kFriday: return (char *) "Пят";
    case Time::kSaturday: return (char *) "Суб";
  }
  return "ННН";
}

/**
 * выводим время и дату в формате.
 */
char * printTime(Time * time, ShowTimeType showMode) {
  switch (showMode) {
  case DataTime:
    snprintf(buffer, sizeof(buffer), "%02d/%02d/%04d %02d:%02d:%02d",
      time->date, time->mon, time->yr, time->hr, time->min, time->sec
    );
    break;
  case DataMode:
    snprintf(buffer, sizeof(buffer), "%s %04d-%02d-%02d ", dayAsString(time->day), time->yr, time->mon, time->date);
    break;
  case TimeMode:
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", time->hr, time->min, time->sec);
    break;
  default:
    buffer[0] = 0;
  }
  return buffer;
}

/**
 * Редактирование времени
 * возвращается true если мы ещё редактируем
 */
bool editTime(char key) {
  static int nField = 0;
  static int nPosit = 0;

  if (key >= '0' && key <= '9') {
    setValue(nField, nPosit, key);
    showField(nField, nPosit);
    key = '>';
  }

  switch(key) {
  case 1: { // начальная
    Time time = rtc.time();
    Time2Fields(time);
    fillPlace(0, 0, 14, textColor);
    screen.print(printTime(&time, DataMode));
    fillPlace(18, 0, 8, textColor);
    screen.print(printTime(&time, TimeMode));
    nField = RT_MAX_FIELDS;
    nPosit = 0;
    showField(nField, nPosit);
  } break;
  case '>':
    nPosit++;
    if (nPosit >= RTFields[nField].len) {
      if (nField < RT_MAX_FIELDS) {
        showField(nField, -1);
        nField++;
        nPosit = 0;
      } else {
        nPosit--;
      }
    }
    showField(nField, nPosit);
    break;
  case '<':
    nPosit--;
    if (nPosit < 0) {
      if (nField > 0) {
        showField(nField, -1);
        nField--;
      }
      nPosit = RTFields[nField].len - 1;
    }
    showField(nField, nPosit);
    break;
  case '+':
  case '-':
    setValue(nField, nPosit, key);
    showField(nField, nPosit);
    break;
  case 'p': // записываем и выходим
    // initialize real time clock.
    rtc.writeProtect(false);
    rtc.halt(false);
    rtc.time(Fields2Time());
    return false;
  case 'b': // выходим без записи
    return false;
  }
  return true;
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
 * рисуем очередной филд
 */
void showField(int nField, int nPosit) {
  RTField field = RTFields[nField];
  char buf[5];
  if (nField == 0) {
    fillPlace(field.col, field.row, field.len, editColorTime);
    screen.print(dayAsString((Time::Day) field.val));
  } else {
    fillPlace(field.col, field.row, field.len, textColor);
    int val = field.val;
    buf[field.len] = 0;
    for (int i = field.len - 1; i >= 0; i--) {
      buf[i] = '0' + val % 10;
      val = val / 10;
    }
    screen.print(buf);
    if (nPosit >= 0 && nPosit <field.len) {
      fillPlace(field.col + nPosit, field.row, 1, editColorTime);
      screen.print(buf[nPosit]);
    }
  }
}

/**
 * устанавливаем значение поля
 */
void setValue(int nField, int nPosit, char key) {
  RTField field = RTFields[nField];
  byte buf[5];
  uint16_t val = field.val;
  for (int i = field.len - 1; i >= 0; i--) {
    buf[i] = val % 10;
    val = val / 10;
  }

  if (key >= '0' && key <= '9') {
    buf[nPosit] = key - '0';
  } else switch (key) {
    case '+':
    buf[nPosit]++;
    break;
    case '-':
    buf[nPosit]--;
    break;
  }

  val = 0;
  for (int i = 0; i < field.len; i++) {
    val *= 10;
    val += buf[i];
  }

  if (val > field.maxVal) {
    val = field.maxVal;
  }
  uint16_t minVal = (nField < 4 ? 1 : 0);
  if (val < minVal) {
    val = minVal;
  }
  RTFields[nField].val = val;
}
