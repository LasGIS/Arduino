#include "LcdPanel.h"
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include "set_screen.h"

extern uint8_t buzzerfactor;
extern char comBuffer[50];
extern LiquidCrystal_I2C lcd;

AlarmClock alarmClocks[4] = {
  AlarmClock(1),
  AlarmClock(2),
  AlarmClock(3),
  AlarmClock(4)
};

const char* toText(const uint8_t val) {
  return val ? "\07" : "-";
}

/**
0123456789abcdef
Buzzer = 1
1v MTWTFSS 24:59
2v MTWTFSS 24:59
3v MTWTFSS 24:59
4v MTWTFSS 24:59
 */
SetScreen::SetScreen(): LcdScreen() {
#ifdef HAS_DEBUG
  name = "SetScreen";
#endif
  commonMaxFields = 0;
  commonFields = new LcdField[commonMaxFields + 1] {
    {0, 9, 1, 0, 8, 6, NULL},   // сила звука
  };
  alarmMaxFields = 9;
  alarmFields = new LcdField[alarmMaxFields + 1] {
    {0,  1, 1, 0, 1,   1, toText},   // active
    {0,  3, 1, 0, 1,   1, toText},   // isMonday
    {0,  4, 1, 0, 1,   1, toText},   // isTuesday
    {0,  5, 1, 0, 1,   1, toText},   // isWednesday
    {0,  6, 1, 0, 1,   1, toText},   // isThursday
    {0,  7, 1, 0, 1,   1, toText},   // isFriday
    {0,  8, 1, 0, 1,   0, toText},   // isSaturday
    {0,  9, 1, 0, 1,   0, toText},   // isSunday
    {0, 11, 2, 0, 23, 23, NULL},   // hour
    {0, 14, 2, 0, 59, 95, NULL},   // min
  };
  load();
}

/**
 * выводим содержимое на LCD.
 */
void SetScreen::showOnce() {
  lcd.clear();
  lcd.setCursor(0, 0);
  int row = 0;
  int i = offset - 1;
  if (offset == 0) {
    lcd.print("Buzzer = ");
    lcd.print(buzzerfactor);
    row++;
    i = 0;
  }
  for (; row < 2 && i < 4; row++, i++) {
    lcd.setCursor(0, row);
    alarmClocks[i].toString(comBuffer, sizeof(comBuffer));
    lcd.print(comBuffer);
  }
}

/**
 * Редактирование настроек
 */
void SetScreen::edit(char key) {
  switch(key) {
  case 1: // начальная
    load();
    break;
  case 'p': // записываем и выходим
    save();
  case 'b': // выходим без записи
    break;
  }
  LcdScreen::edit(key);
}

void SetScreen::control(char key) {
  switch (key) {
  case '+':
    if (offset > 0) {
      offset--;
    } else {
      editRow = 0;
    }
    break;
  case '-':
    if (offset < 3) {
      offset++;
    } else {
      editRow = 1;
    }
    break;
  case 'p':
    edit(1);
    break;
  }
}

/**
 * вышли за пределы дозволенной области редактирования
 */
void SetScreen::hasBeyond(char key) {
  save();
  if (key == '<') {
    if (editRow > 0) {
      editRow--;
    } else if (offset > 0) {
      offset--;
      editRow = 0;
    } else {
      offset = 0;
      editRow = 0;
    }
  } else if (key == '>') {
    if (editRow < 1) {
      editRow++;
    } else if (offset < 3) {
      offset++;
      editRow = 0;
    } else {
      offset = 3;
      editRow = 1;
    }
  }
  load();
  showOnce();
}

void SetScreen::save() {
  if (offset == 0 && editRow == 0) {
    buzzerfactor = fields[0].val;
    EEPROM.update(BUZZER_FACTOR_ADR, buzzerfactor);
  } else {
    AlarmClock * alarm = &alarmClocks[offset + editRow - 1];
    alarm->active      = fields[0].val;
    alarm->isMonday    = fields[1].val;
    alarm->isTuesday   = fields[2].val;
    alarm->isWednesday = fields[3].val;
    alarm->isThursday  = fields[4].val;
    alarm->isFriday    = fields[5].val;
    alarm->isSaturday  = fields[6].val;
    alarm->isSunday    = fields[7].val;
    alarm->hour        = fields[8].val;
    alarm->min         = fields[9].val;
  }
}

void SetScreen::load() {
  if (offset == 0 && editRow == 0) {
    maxFields = commonMaxFields;
    fields = commonFields;
    fields[0].val = buzzerfactor;
  } else {
    maxFields = alarmMaxFields;
    fields = alarmFields;
    for (int i = 0; i <= maxFields; i++) {
      fields[i].row = editRow;
    }
    AlarmClock * alarm = &alarmClocks[offset + editRow - 1];
    fields[0].val = alarm->active;
    fields[1].val = alarm->isMonday;
    fields[2].val = alarm->isTuesday;
    fields[3].val = alarm->isWednesday;
    fields[4].val = alarm->isThursday;
    fields[5].val = alarm->isFriday;
    fields[6].val = alarm->isSaturday;
    fields[7].val = alarm->isSunday;
    fields[8].val = alarm->hour;
    fields[9].val = alarm->min;
  }
}

