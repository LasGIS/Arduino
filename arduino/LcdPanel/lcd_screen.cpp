#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "lcd_screen.h"

extern LiquidCrystal_I2C lcd;

/**
 * устанавливаем значение поля
 */
void LcdField::setValue(int nPosit, char key) {
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
    buf[nPosit]--;
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
}

/**
 * рисуем очередной филд
 */
void LcdField::showField(int nPosit) {
  char buf[5];
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
}
