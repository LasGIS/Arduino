#ifndef LCD_PANEL_H
#define LCD_PANEL_H

#include <Arduino.h>

#define CUR_COMMAND_ADR 0
#define SHOW_MODE_ADR 1
#define BUZZER_FACTOR_ADR 2

// установка режима вывода отладочной информации
//#define HAS_SERIAL

#define CURRENT_COMMAND_TYPE_MAX 3
/* текущий экран. */
enum CurrentCommandType : uint8_t {
  mainCommand = 0,
  settingsScreen= 1,
  showLCDchars = 2,
  showIRkey = 3/*,
  showDistance = 4 */
};

void buzzerOut(unsigned int hertz, unsigned long del);
//void buzzerOutTest(unsigned int hertz, unsigned long del);
void loadCustomChars();
void viewCustomDigit(int offset, int digit);

#endif // LCD_PANEL_H
