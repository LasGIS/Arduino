#ifndef LCD_PANEL_H
#define LCD_PANEL_H

#include <Arduino.h>
#include <EEPROM.h>
#include <IrControl.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>
#include <DHT.h>

enum LPModeType : uint8_t {
  show, edit
};
enum LPShowModeType : uint8_t {
  BigTime = 0,
  DataTime = 1,
  TimeHum = 2,
  Humidity = 3,
  Battery = 4
};
enum CurrentCommandType : uint8_t {
  mainCommand = 0,
  showLCDchars = 1,
  showIRkey = 2/*,
  showDistance = 3 */
};

void buzzerOut(unsigned int hertz, unsigned long del);
//void buzzerOutTest(unsigned int hertz, unsigned long del);
LPModeType editTime(char key);
void printTime(LPShowModeType showMode);
void loadCustomChars();
void viewCustomDigit(int offset, int digit);

#endif // LCD_PANEL_H
