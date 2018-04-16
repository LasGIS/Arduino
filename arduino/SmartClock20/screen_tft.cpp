#include "SmartClock20.h"

extern LPModeType mode;

/**
 * ������������� �������� ����
 */
void FieldTft::setValue(int nPosit, char key) {
/*
  byte buf[5];
  uint16_t _val = val;
  for (int i = len - 1; i >= 0; i--) {
    buf[i] = _val % 10;
    _val = _val / 10;
  }

  if (key >= '0' && key <= '9') {
    buf[nPosit] = key - '0';
  } else switch (key) {
    case '+':
    buf[nPosit]++;
    break;
    case '-':
    if (buf[nPosit] > 0) buf[nPosit]--;
    break;
  }

  _val = 0;
  for (int i = 0; i < len; i++) {
    _val *= 10;
    _val += buf[i];
  }

  if (_val > maxVal) {
    _val = maxVal;
  }
  if (_val < minVal) {
    _val = minVal;
  }
  val = _val;
*/
}

/**
 * ������ ��������� ����
 */
void FieldTft::showField(int nPosit) {
/*  char buf[5];
  lcd.setCursor(col, row);
  if (getValue != NULL) {
    lcd.print(getValue(val));
  } else {
    int _val = val;
    buf[len] = 0;
    for (int i = len - 1; i >= 0; i--) {
      buf[i] = '0' + _val % 10;
      _val = _val / 10;
    }
    lcd.print(buf);
  }
  lcd.setCursor(col + nPosit, row);
*/
}

ScreenTft::ScreenTft() {
  nField = 0;
  nPosit = 0;
}

void ScreenTft::showEveryTime() {
}

void ScreenTft::showOnce() {
}

void ScreenTft::control(char key) {
}

void ScreenTft::edit(char key) {
#ifdef HAS_SERIAL
  Serial.print(nField);
  Serial.print(", ");
  Serial.print(nPosit);
  Serial.println(";");
#endif

  if (key >= '0' && key <= '9') {
    fields[nField].setValue(nPosit, key);
    fields[nField].showField(nPosit);
    key = '>';
  }
/*
  switch(key) {
  case 1: // ���������
    lcd.clear();
    showOnce();
    lcd.cursor();
    lcd.blink();
    nField = maxFields;
    nPosit = 0;
    mode = LPModeType::edit;
    break;
  case '>':
    nPosit++;
    if (nPosit >= fields[nField].len) {
      if (nField < maxFields) {
        nField++;
        nPosit = 0;
      } else {
        nPosit--;
        hasBeyond(key);
      }
    }
    break;
  case '<':
    nPosit--;
    if (nPosit < 0) {
      if (nField > 0) {
        nField--;
      } else {
        nPosit++;
        hasBeyond(key);
      }
      nPosit = fields[nField].len - 1;
    }
    break;
  case '+':
  case '-':
    fields[nField].setValue(nPosit, key);
    break;
  case 'p': // ���������� � �������
  case 'b': // ������� ��� ������
    lcd.noCursor();
    lcd.noBlink();
    mode = LPModeType::show;
    return;
  }
  fields[nField].showField(nPosit);
*/
}
