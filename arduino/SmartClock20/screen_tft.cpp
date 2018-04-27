#include "SmartClock20.h"

/**
 * выводим батарейки.
 */
void printVolts() {
  // 1 сборка
  double vBattery = analogRead(A7) * 0.00664;
  double vCharger = analogRead(A6) * 0.00664;
  // 2 сборка
//  double vBattery = analogRead(A7) * 0.00661;
//  double vCharger = analogRead(A6) * 0.00654;
  // 3 сборка
//  double vBattery = analogRead(A7) * 0.00631;
//  double vCharger = analogRead(A6) * 0.00630;
#ifdef HAS_SERIAL
  Serial.print("vBattery = ");
  Serial.println(vBattery);
  Serial.print("vCharger = ");
  Serial.println(vCharger);
#endif
  drawDouble(4, 1, vBattery, COLOR_BLUE);
  drawDouble(15, 1, vCharger, COLOR_BLUEVIOLET);
}

/**
 * устанавливаем значение поля
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
 * рисуем очередной филд
 */
void FieldTft::showField(int nPosit) {
  char buf[5];
  if (getValue != NULL) {
    printText(col, row, getValue(val), COLOR_INDIGO);
  } else {
    int _val = val;
    buf[len] = 0;
    for (int i = len - 1; i >= 0; i--) {
      buf[i] = '0' + _val % 10;
      _val = _val / 10;
    }
    printText(col, row, buf, COLOR_INDIGO);
  }
  setCursor(col + nPosit, row);
}

ScreenTft::ScreenTft() {
  nField = 0;
  nPosit = 0;
}

void ScreenTft::changeOrientation(OrientationType orientation) {
#ifdef HAS_SERIAL
  Serial.println(orientation);
#endif
  if (orientation & ORIENTATION_HORISONTAL) {
    X0 = BOXH_X0;
    X1 = BOXH_X1;
    Y0 = BOXH_Y0;
    Y1 = BOXH_Y1;
    clockX = CLOCKH_X;
    clockY = CLOCKH_Y;
    ClockX0 = BOXCLOCKH_X0;
    ClockX1 = BOXCLOCKH_X1;
    ClockY0 = BOXCLOCKH_Y0;
    ClockY1 = BOXCLOCKH_Y1;
    boxCenterX = BOXH_CENTER_X;
    boxCenterY = BOXH_CENTER_Y;
  } else {
    X0 = BOXV_X0;
    X1 = BOXV_X1;
    Y0 = BOXV_Y0;
    Y1 = BOXV_Y1;
    clockX = CLOCKV_X;
    clockY = CLOCKV_Y;
    ClockX0 = BOXCLOCKV_X0;
    ClockX1 = BOXCLOCKV_X1;
    ClockY0 = BOXCLOCKV_Y0;
    ClockY1 = BOXCLOCKV_Y1;
    boxCenterX = BOXV_CENTER_X;
    boxCenterY = BOXV_CENTER_Y;
  }
}

void ScreenTft::showTime(DateTime * dateTime) {
  printShortTime(
    dateTime->hour(),
    dateTime->minute(),
    dateTime->second()
  );
  printRealDate();
  printVolts();
}

void ScreenTft::showEveryTime() {
}

void ScreenTft::showOnce() {
  tft.drawRectangle(X0, Y0, X1, Y1, COLOR_WHITE);
//    tft.drawRectangle(ClockX0, ClockY0, ClockX1, ClockY1, COLOR_WHITE);
//    tft.fillRectangle(ClockX0 + 1, ClockY0 + 1, ClockX1 - 1, ClockY1 - 1, COLOR_GRAY);
//  #ifdef ADXL345_ENABLED
//    printText(0,  2, "X=", COLOR_GRAY);
//    printText(8,  2, "Y=", COLOR_GRAY);
//    printText(16, 2, "Z=", COLOR_GRAY);
//  #endif
  printText(0,  1, "Бат.", COLOR_GRAY);
  printText(11, 1, "Зар.", COLOR_GRAY);
  printText(22, 1, name, COLOR_RED);
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
  switch(key) {
  case 1: // начальная
    mode = ModeType::edit;
    break;
  case '>':
    nPosit++;
    if (nPosit >= fields[nField].len) {
      if (nField < maxFields) {
        nField++;
        nPosit = 0;
      } else {
        nPosit--;
//        hasBeyond(key);
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
//        hasBeyond(key);
      }
      nPosit = fields[nField].len - 1;
    }
    break;
  case '+':
  case '-':
    fields[nField].setValue(nPosit, key);
    break;
  case 'M': // записываем и выходим
  case 'r': // выходим без записи
    mode = ModeType::show;
    return;
  }
  fields[nField].showField(nPosit);
}
