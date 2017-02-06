#include "LcdPanel.h"
#include <LiquidCrystal_I2C.h>
#include "show_char_screen.h"

extern LiquidCrystal_I2C lcd;

ShowCharScreen::ShowCharScreen(): LcdScreen() { }

/**
 * Показываем последовательно раскладку
 */
void ShowCharScreen::showOnce() {
  int row = charsRow;
  for (int i = 0; i < 2; i++, row++) {
    lcd.setCursor(0, i);
    if (row > 16) {
      row = 0;
    }
    for (int col = 0; col < 16; col++) {
      lcd.write(row * 16 + col);
    }
  }
}

void ShowCharScreen::control(char key) {
  switch (key) {
  case '-':
    if (charsRow >= 15) {
      charsRow = 0;
    } else {
      charsRow++;
    }
    break;
  case '+':
    if (charsRow == 0) {
      charsRow = 15;
    } else {
      charsRow--;
    }
    break;
  case 'p':
    musicAlarm();
    break;
  }
}
