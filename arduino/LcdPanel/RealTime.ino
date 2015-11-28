#define RT_MAX_FIELDS 6

class RTField {
public:
  uint8_t row;
  uint8_t col;
  uint8_t len;
  byte buf[4];
};

RTField RTFields[RT_MAX_FIELDS + 1] {
  {0, 0, 3, {'s', 'a', 't', '0'}},
  {0, 4, 4, {'2', '0', '1', '5'}},
  {0, 9, 2, {'0', '0', '0', '0'}},
  {0, 12, 2, {'0', '0', '0', '0'}},
  {1, 7, 2, {'0', '0', '0', '0'}},
  {1, 10, 2, {'0', '0', '0', '0'}},
  {1, 13, 2, {'0', '0', '0', '0'}}
};

String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: { return "Sun"; }
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
    break;
  case '-':
    break;
  case 'p': // записываем и выходим
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
  
}

Time Fields2Time() {
  Time tm(2099, 1, 1, 0, 0, 0, Time::kSunday);
  return tm;
}

/**
 * рисуем очередной филд
 */
void showField(int nField, int nPosit) {
  RTField field = RTFields[nField];
  lcd.setCursor(field.col, field.row);
  for (int i = 0; i < field.len; i++) {
    lcd.write(field.buf[i]);
  }
  lcd.setCursor(field.col + nPosit, field.row);
}

void setValue(int nField, int nPosit, char val) {
  RTFields[nField].buf[nPosit] = val;
}


