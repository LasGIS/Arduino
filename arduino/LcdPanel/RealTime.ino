#define RT_MAX_FIELDS 6

class RTField {
public:
  uint8_t row;
  uint8_t col;
  uint8_t len;
  uint16_t maxVal;
  uint16_t val;
};

RTField RTFields[RT_MAX_FIELDS + 1] {
  {0, 0, 1, 7, 1},
  {0, 4, 4, 9999, 2015},
  {0, 9, 2, 12, 1},
  {0, 12, 2, 31, 1},
  {1, 7, 2, 23, 0},
  {1, 10, 2, 59, 0},
  {1, 13, 2, 59, 0}
};

String dayAsString(const Time::Day day) {
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

void printTime(int showMode) {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  // Name the day of the week.
  const String day = dayAsString(t.day);

  // Format the time and date and insert into the temporary buffer.
  char buf[50];
  if (showMode == 0) {
    snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d ", day.c_str(), t.yr, t.mon, t.date);
    lcd.setCursor(0, 0);
    lcd.print(buf);
  }
  //Serial.print(buf);
  if (showMode < 2) {
    snprintf(buf, sizeof(buf), "Time = %02d:%02d:%02d", t.hr, t.min, t.sec);
    lcd.setCursor(0, showMode == 0 ? 1 : 0);
    lcd.print(buf);
  }
  //Serial.println(buf);
}

/**
 * Редактирование времени
 */
LPMode editTime(char key) {
  static int nField = 0;
  static int nPosit = 0;

  if (key >= '0' && key <= '9') {
    setValue(nField, nPosit, key);
    showField(nField, nPosit);
    key = '>';
  }

  switch(key) {
  case 1: // начальная 
    Time2Fields(rtc.time());
    lcd.clear();
    printTime(0);
    lcd.cursor();
    lcd.blink();
    nField = 0;
    nPosit = 0;
    showField(nField, nPosit);
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
    showField(nField, nPosit);
    break;
  case '<':
    nPosit--;
    if (nPosit < 0) {
      if (nField > 0) {
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
    rtc.time(Fields2Time());
    lcd.noCursor();
    lcd.noBlink();
    return show;
  case 'b': // выходим без записи
    lcd.noCursor();
    lcd.noBlink();
    return show;
  }
  return edit;
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
  lcd.setCursor(field.col, field.row);
  if (nField == 0) {
    lcd.print(dayAsString((Time::Day) field.val));
  } else {
    int val = field.val;
    buf[field.len] = 0;
    for (int i = field.len - 1; i >= 0; i--) {
      buf[i] = '0' + val % 10;
      val = val / 10;
    }
    lcd.print(buf);
  }
  lcd.setCursor(field.col + nPosit, field.row);
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
  int minVal = (nField < 4 ? 1 : 0);
  if (val < minVal) {
    val = minVal;
  }
  RTFields[nField].val = val;
}
