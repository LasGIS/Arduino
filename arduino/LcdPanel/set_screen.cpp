#include "LcdPanel.h"
#include "set_screen.h"

extern char comBuffer[50];
extern LiquidCrystal_I2C lcd;

SetScreen::SetScreen() {
  zummer = 100;
}

/***
 * выводим содержимое на LCD.
 */
void SetScreen::show() {
  snprintf(comBuffer, sizeof(comBuffer), "Zummer = %02d", zummer);
  lcd.setCursor(0, 0);
  lcd.print(comBuffer);
}

/***
 * Редактирование настроек
 */
LPModeType SetScreen::edit(char key) {
  /*
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
    printTime(DataTime);
    lcd.cursor();
    lcd.blink();
    nField = RT_MAX_FIELDS;
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
    // initialize real time clock.
    rtc.writeProtect(false);
    rtc.halt(false);
    rtc.time(Fields2Time());
    lcd.noCursor();
    lcd.noBlink();
    return show;
  case 'b': // выходим без записи
    lcd.noCursor();
    lcd.noBlink();
    return show;
  }
  */
  return LPModeType::edit;
}
