#ifndef LCD_PANEL_H
#define LCD_PANEL_H

#include <Arduino.h>
#include <IrControl.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>
#include <DHT.h>

enum LPModeType : uint8_t {
  show, edit
};
/* модель отображения для главного экрана */
enum LPShowModeType : uint8_t {
  BigTime = 0,
  DataTime = 1,
  TimeHum = 2,
  Humidity = 3,
  Battery = 4
};
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
void printTime(LPShowModeType showMode);
void loadCustomChars();
void viewCustomDigit(int offset, int digit);

#endif // LCD_PANEL_H
