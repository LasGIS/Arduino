/*************************************
 * Отображение больших цифр на дисплее.
 *************************************/
#include "LcdPanel.h"
#include <LiquidCrystal_I2C.h>

static uint8_t FIRST_LCD_CHARS[8][8] = {
  { B00111, B01111, B11111, B11111, B11111, B11111, B11111, B11111 },
  { B11111, B11111, B11111, B00000, B00000, B00000, B00000, B00000 },
  { B11100, B11110, B11111, B11111, B11111, B11111, B11111, B11111 },
  { B11111, B11111, B11111, B11111, B11111, B11111, B01111, B00111 },
  { B00000, B00000, B00000, B00000, B00000, B11111, B11111, B11111 },
  { B11111, B11111, B11111, B11111, B11111, B11111, B11110, B11100 },
  { B11111, B11111, B11111, B00000, B00000, B00000, B11111, B11111 },
  { B00001, B00011, B10011, B10110, B10110, B01110, B01110, B00110 }
};
static uint8_t CUSTOM_DIGITS[10][2][3] {
  { { 0,  1,  2}, { 3,  4,  5} },  // 0
  { { 1,  2, 32}, { 4,255,  4} },  // 1
  { { 6,  6,  2}, { 3,  4,  4} },  // 2
  { { 6,  6,  2}, { 4,  4,  5} },  // 3
  { { 3,  4,255}, {32, 32,255} },  // 4
  { { 3,  6,  6}, { 4,  4,  5} },  // 5
  { { 0,  6,  6}, { 3,  4,  5} },  // 6
  { { 1,  1,  2}, {32, 32,255} },  // 7
  { { 0,  6,  2}, { 3,  4,  5} },  // 8
  { { 0,  6,  2}, { 4,  4,  5} }   // 9
};
extern LiquidCrystal_I2C lcd;

void loadCustomChars() {
  for (int i = 0; i < 8; i++) {
    lcd.createChar(i, FIRST_LCD_CHARS[i]);
  }
}

void viewCustomDigit(int offset, int digit) {
  for (int row = 0; row < 2; row++) {
    lcd.setCursor(offset, row);
    for (int i = 0; i < 3; i++) {
      lcd.write(CUSTOM_DIGITS[digit][row][i]);
    }
  }
}
