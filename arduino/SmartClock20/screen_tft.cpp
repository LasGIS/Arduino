#include "SmartClock20.h"

/**
 * выводим батарейки.
 */
void printVolts() {
  LoadClass lc(EEPROM_DEVICE, EEPROM_factorBattery);
  float factorBattery = lc.readFloat();
  float factorCharger = lc.readFloat();

  // 1 сборка
  //  float vBattery = analogRead(A7) * 0.00664;
  //  float vCharger = analogRead(A6) * 0.00664;
  // 2 сборка
  //  float vBattery = analogRead(A7) * 0.00661;
  //  float vCharger = analogRead(A6) * 0.00654;
  // 3 сборка 
  float vBattery = analogRead(A7) * factorBattery;
  float vCharger = analogRead(A6) * factorCharger;
#ifdef HAS_SERIAL
  Serial.print("vBattery = ");
  Serial.println(vBattery);
  Serial.print("vCharger = ");
  Serial.println(vCharger);
#endif
  drawDouble(1, 1, 1, vBattery, COLOR_BLUE);
  drawDouble(8, 1, 1, vCharger, COLOR_BLUEVIOLET);
}

/**
 * устанавливаем значение поля
 */
void FieldTft::setValue(int8_t nPosit, char key) {
  uint16_t _val = val;
  for (int i = len - 1; i >= 0; i--) {
    comBuffer[i] = _val % 10;
    _val = _val / 10;
  }

  if (key >= '0' && key <= '9') {
    comBuffer[nPosit] = key - '0';
  } else switch (key) {
  case '+':
    comBuffer[nPosit]++;
    break;
  case '-':
    if (comBuffer[nPosit] > 0) comBuffer[nPosit]--;
    break;
  }

  _val = 0;
  for (int i = 0; i < len; i++) {
    _val *= 10;
    _val += comBuffer[i];
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
void FieldTft::showField(int8_t nPosit) {
  if (getValue != NULL) {
    printText(col, row, fontSize, getValue(val), COLOR_WHITE);
  } else {
    int _val = val;
    comBuffer[len] = 0;
    for (int i = len - 1; i >= 0; i--) {
      comBuffer[i] = '0' + _val % 10;
      _val = _val / 10;
    }
    printText(col, row, fontSize, comBuffer, COLOR_WHITE);
  }
  if (nPosit >= 0) {
    setCursor(col + nPosit, row, fontSize);
  }
}

ScreenTft::ScreenTft() {
  nField = 0;
  nPosit = 0;
}

ScreenTft::ScreenTft(int16_t address) {
  int len;
  loadClass.toAddress(address);
  ScreenTftStruct * screenTft = loadClass.readObject(len);
#ifdef HAS_SERIAL
  Serial.print("screenTft(");
  Serial.print(len);
  Serial.print("): ");
  SerialPrintHex((uint8_t *) screenTft, len);
  Serial.println();
  Serial.print("name:");
  Serial.println(screenTft->name);
  Serial.print("maxFields:");
  Serial.println(screenTft->maxFields);
  Serial.print("fields(");
  SerialPrintHex((uint8_t*) screenTft->fields, (screenTft->maxFields+1) * 2);
  Serial.println("): [");
  for (int i = 0; i <= screenTft->maxFields; i++) {
    SerialPrintHex((uint8_t*) (screenTft->fields[i]), 2);
    Serial.print("  { row: ");
    Serial.print(screenTft->fields[i]->row);
    Serial.print(", col: ");
    Serial.print(screenTft->fields[i]->col);
    Serial.print(", fontSize: ");
    Serial.print(screenTft->fields[i]->fontSize);
    Serial.print(", len: ");
    Serial.print(screenTft->fields[i]->len);
    Serial.print(", minVal: ");
    Serial.print(screenTft->fields[i]->minVal);
    Serial.print(", maxVal: ");
    Serial.print(screenTft->fields[i]->maxVal);
    Serial.print(", val: ");
    Serial.print(screenTft->fields[i]->val);
    Serial.println("},");
  }
  Serial.println("]");
#endif
  name = screenTft->name;
  nField = 0;
  nPosit = 0;
  maxFields = screenTft->maxFields;
  fields = new FieldTft[maxFields + 1];
  for (int i = 0; i <= screenTft->maxFields; i++) {
    fields[i].row = screenTft->fields[i]->row;
    fields[i].col = screenTft->fields[i]->col;
    fields[i].fontSize = screenTft->fields[i]->fontSize;
    fields[i].len = screenTft->fields[i]->len;
    fields[i].minVal = screenTft->fields[i]->minVal;
    fields[i].maxVal = screenTft->fields[i]->maxVal;
    fields[i].val = screenTft->fields[i]->val;
    fields[i].getValue = NULL;
  }
}

void ScreenTft::changeOrientation() {
#ifdef HAS_SERIAL
  Serial.println(tft.getOrientation());
#endif
  if (isHorisontalOrientation()) {
    X0 = BOXH_X0;
    X1 = BOXH_X1;
    Y0 = BOXH_Y0;
    Y1 = BOXH_Y1;
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
    ClockX0 = BOXCLOCKV_X0;
    ClockX1 = BOXCLOCKV_X1;
    ClockY0 = BOXCLOCKV_Y0;
    ClockY1 = BOXCLOCKV_Y1;
    boxCenterX = BOXV_CENTER_X;
    boxCenterY = BOXV_CENTER_Y;
  }
}

void ScreenTft::showTime(DateTime * dateTime) {
  printShortTime(dateTime);
  printShortDate(dateTime);
  printDayOfWeek(dateTime);
  printVolts();
}

void ScreenTft::showEveryTime() {
}

void ScreenTft::showOnce() {
  tft.fillRectangle(X0, Y0, X1, Y1, COLOR_BLACK);
  tft.drawRectangle(X0, Y0, X1, Y1, COLOR_WHITE);
  //  tft.fillRectangle(ClockX0 + 1, ClockY0 + 1, ClockX1 - 1, ClockY1 - 1, COLOR_GRAY);
  //  tft.drawRectangle(ClockX0, ClockY0, ClockX1, ClockY1, COLOR_WHITE);
  //#ifdef ADXL345_ENABLED
  //  printText(0,  2, 1, "X=", COLOR_GRAY);
  //  printText(8,  2, 1, "Y=", COLOR_GRAY);
  //  printText(16, 2, 1, "Z=", COLOR_GRAY);
  //#endif
  printText(0,  1, 1, "V", COLOR_GRAY);
  printText(7,  1, 1, "/", COLOR_GRAY);
  printText(14, 1, 1, name, COLOR_RED);
  isRedraw = true;
}

void ScreenTft::control(char key __attribute__ ((unused))) { }

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
      }
    }
    break;
  case '<':
    nPosit--;
    if (nPosit < 0) {
      if (nField > 0) {
        nField--;
        nPosit = fields[nField].len - 1;
      } else {
        nPosit++;
      }
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
  //showCurrentField;
}

/** показать текущее поле */
void ScreenTft::showCurrentField() {
  fields[nField].showField(nPosit);
}

/** показать все поля */
void ScreenTft::showAllFields() {
  for (uint8_t i = 0; i <= maxFields; i++) {
    fields[i].showField();
  }
}
