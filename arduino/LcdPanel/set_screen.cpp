#include "LcdPanel.h"
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include "set_screen.h"

#define SS_MAX_FIELDS 0

extern uint8_t buzzerfactor;
//extern char comBuffer[50];
extern LiquidCrystal_I2C lcd;

SetScreen::SetScreen() {
  maxFields = SS_MAX_FIELDS;
  fields = new LcdField[maxFields + 1];
  fields[0] = {0, 9, 1, 0, 8, 6, NULL};   // сила звука
//  fields[1] = {1, 13, 2, 0, 59, 0, NULL}; // секунда
}

/**
 * выводим содержимое на LCD.
 */
void SetScreen::showOnce() {
  LcdScreen::showOnce();
  lcd.print("Buzzer = ");
  lcd.print(buzzerfactor);
}

/**
 * Редактирование настроек
 */
void SetScreen::edit(char key) {
  switch(key) {
  case 1: // начальная
    fields[0].val = buzzerfactor;
    break;
  case 'p': // записываем и выходим
    buzzerfactor = fields[0].val;
    EEPROM.update(BUZZER_FACTOR_ADR, buzzerfactor);
  case 'b': // выходим без записи
    break;
  }
  LcdScreen::edit(key);
}
