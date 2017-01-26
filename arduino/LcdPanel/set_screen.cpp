#include "LcdPanel.h"
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include "set_screen.h"

#define SS_MAX_FIELDS 0

extern uint8_t buzzerfactor;
//extern char comBuffer[50];
extern LiquidCrystal_I2C lcd;

LcdField SSFields[SS_MAX_FIELDS + 1] {
  {0, 9, 1, 0, 8, 6, NULL},         // сила звука
  //{1, 13, 2, 0, 59, 0, NULL}          // секунда
};

SetScreen::SetScreen() {
}

/***
 * выводим содержимое на LCD.
 */
void SetScreen::show() {
  lcd.setCursor(0, 0);
  lcd.print("Buzzer = ");
  lcd.print(buzzerfactor);
}

/***
 * Редактирование настроек
 */
LPModeType SetScreen::edit(char key) {
  static int nField = 0;
  static int nPosit = 0;
#ifdef HAS_SERIAL
  Serial.print(nField);
  Serial.print(", ");
  Serial.print(nPosit);
  Serial.println(";");
#endif

  if (key >= '0' && key <= '9') {
    SSFields[nField].setValue(nPosit, key);
    SSFields[nField].showField(nPosit);
    key = '>';
  }

  switch(key) {
  case 1: // начальная
    lcd.clear();
    show();
    SSFields[0].val = buzzerfactor;
    lcd.cursor();
    lcd.blink();
    nField = SS_MAX_FIELDS;
    nPosit = 0;
    break;
  case '>':
    nPosit++;
    if (nPosit >= SSFields[nField].len) {
      if (nField < SS_MAX_FIELDS) {
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
      nPosit = SSFields[nField].len - 1;
    }
    break;
  case '+':
  case '-':
    SSFields[nField].setValue(nPosit, key);
    break;
  case 'p': // записываем и выходим
    buzzerfactor = SSFields[0].val;
    EEPROM.update(BUZZER_FACTOR_ADR, buzzerfactor);
  case 'b': // выходим без записи
    lcd.noCursor();
    lcd.noBlink();
    return LPModeType::show;
  }
  SSFields[nField].showField(nPosit);
  return LPModeType::edit;
}
